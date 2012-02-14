#pragma once
#include "nrvExe\Task.h"
#include "nrvApp\NervePluginFactory.h"
#include "nrvApp\NervePluginBase.h"
#include "nrvApp\PluginHandler.h"
#include "nrvService\ServiceBinding.h"
#include "nrvService\NerveService.h"
#include <QtGui\QDialog>
#include "nrvApp\TaskAdapterGui.h"
#include <vector>
#include "nrv\MessageQueue.h"
#include "nrv/EventObserver.h"
#include "nrvApp/NerveApplication.h"
#include "nrvToolbox/RebroadcastManager.h"
#include "nrv/GuiTools.h"
#include "nrvToolbox/FunctionQueueQt.h"
#include <osg/Camera>
#include "nrvService/Invoker.h"
#include "nrvService/InvokeMapper.h"

class SettingsGui : public QDialog
{
};
class TaskAdapter;
class TaskAdapterFactory;



class TaskAdapterAPI : public ServiceBinding
{
public:
	TaskAdapterAPI()
	{
		Invoker<TaskAdapterAPI,void,double, double, double>* invoker = 
			new Invoker<TaskAdapterAPI,void,double,double,double>(this,&TaskAdapterAPI::inputXYZ,"x,y,z");
		invokeMapper.insert("inputXYZ",invoker);
	}
	bool invoke(std::string name, GenericArgList &argList)
	{
		return invokeMapper.mapInvocation(name,argList);
	}
	void inputXYZ(double x, double y, double z)
	{
		printf("invoke inputXYZ(%f,%f,%f)\n",x,y,z);
	}
	 bool testArgumentList(std::string invocableName, GenericArgList const& argList)
	 {
		 return invokeMapper.testArgs(invocableName,argList);
	 }
private:
	InvokeMapper invokeMapper;
	friend class TaskAdapterAPIFactory;
};
class TaskAdapterAPIFactory : public ServiceFactory
{
public:
	TaskAdapterAPIFactory():metaInfo(0){}
	//Meta information accessors
	const ServiceMetaInfo&				getServiceMetaInfo()
	{
		if(metaInfo) return *metaInfo;
		else
		{
			metaInfo = new ServiceMetaInfo("Input","Service allows input of 3 doubles (treated as x,y,z by the task)");
			TaskAdapterAPI api;
			InvokeMapper::addInvokeMapperToMetaInfo(*metaInfo, api.invokeMapper);
			return *metaInfo;
		}
	}

	//Service invoker access
	ServiceBinding*				getBinding(){return new TaskAdapterAPI();}
	void						cleanupBinding(ServiceBinding* binding){delete binding;}
	~TaskAdapterAPIFactory(){delete metaInfo;}
private:
	ServiceMetaInfo* metaInfo;
};
class TaskAdapterFactory : public NervePluginFactory
{
public:
	TaskAdapterFactory(Task* t):_t(t){}
	NervePluginBase* createPluginObject(NerveAPI* appInterface);
	void cleanUpPluginObject(NervePluginBase* plugin,NerveAPI* appInterface)
	{
		QWidget* ui = appInterface->removeUI(0);
		delete plugin;
	}
	QDialog* createInterfaceObject(int i=0)
	{
		switch(i)
		{
		case 0: return createSettingsGui(); break;
		case 1: return new TaskAdapterGui(); break;
		default: return 0;
		}
	}
	void cleanUpInterfaceObject(QDialog* ui){delete ui;}
	const char* getName(){return "Legacy Task";}
	Task* createTask(){return _t->spawnTask(0);}
	SettingsGui* createSettingsGui(){return _t->spawnGui(0);}
private:
	Task* _t;
};
struct LoadCFG : public FunctionQueueQt::Functionoid
{
	LoadCFG(QDialog* gui,std::string str,bool file):g(gui),s(str),f(file){}
	~LoadCFG(){}
	void operator()()
	{
		if(f) GuiToolsClass::readConfigFile(g,s);
		else  GuiToolsClass::readConfigQString(g,&QString::fromStdString(s));
	}
	QDialog* g;
	std::string s;
	bool f;
};
struct AddRenderer : public FunctionQueueQt::Functionoid
{
	AddRenderer(std::string provider,std::string service,unsigned int id, int numScenes,int index,TaskAdapterGui* tag):
		p(provider),s(service),_id(id), n(numScenes),i(index),g(tag){}
	~AddRenderer(){}
	void operator()()
	{
		if(g) g->addRenderer(p.c_str(),s.c_str(),_id,n,i);
	}
	std::string p;
	std::string s;
	int n;
	int i;
	unsigned int _id;
	TaskAdapterGui* g;
};
struct RemoveRenderer : public FunctionQueueQt::Functionoid
{
	RemoveRenderer(unsigned int id,TaskAdapterGui* tag):
		_id(id),g(tag){}
	~RemoveRenderer(){}
	void operator()()
	{
		if(g) g->removeRenderer(_id);
	}
	unsigned int _id;
	TaskAdapterGui* g;
};
struct SendSceneRootList : public FunctionQueueQt::Functionoid
{
	SendSceneRootList(std::vector<std::string> names,TaskAdapterGui* tag):v(names),g(tag){}
	~SendSceneRootList(){}
	void operator()()
	{
		if(g) g->setSceneRootList(v);
	}
	std::vector<std::string> v;
	TaskAdapterGui* g;
};
struct PreviewGui : public FunctionQueueQt::Functionoid
{
	PreviewGui(TaskAdapterFactory* factory,TaskAdapterGui* gui, std::string str, bool file):_factory(factory),g(gui),s(str),f(file){}
	void operator()()
	{
		QDialog* preview = _factory->createInterfaceObject(0);
		g->preview(preview,s,f);
	}
	TaskAdapterFactory* _factory;
	TaskAdapterGui* g;
	std::string s;
	bool f;
};
class TaskAdapter : public NervePluginBase, public UsesSlots
{
public:
	TaskAdapter(TaskAdapterFactory* taf, NerveAPI* papi):
		factory(taf),
		task(0),
		pAPI(papi),
		runStatus(false),
		completedStatus(false),
		launchStatus(false),
		loadConfigFileAtLaunch(false),
		loadConfigStringAtLaunch(false),
		serviceAvailableObserver(this,&TaskAdapter::serviceAvailable),
		serviceUnavailableObserver(this,&TaskAdapter::serviceUnavailable)
	{
		tag = new TaskAdapterGui();
		pAPI->exposeUI(tag,"Adapter");
		pAPI->useThreadedMode();
		//tag = static_cast<TaskAdapterGui*>(pAPI->createRegisterAndExposeUI(1,"Command"));
		tag->rebroadcastManager.signal_broadcastCompleteThreadsafe.connect(this,&TaskAdapter::calledFromGuiThread);
		rebroadcastManager = tag->rebroadcastManager.createReciprocallyConnectedManager();
		rebroadcastManager->getRebroadcastVoid("launchButton")->signal_toSameThread.connect(this,&TaskAdapter::launchButton);
		rebroadcastManager->getRebroadcastVoid("runButton")->signal_toSameThread.connect(this,&TaskAdapter::runButton);
		rebroadcastManager->getRebroadcastVoid("applyButton")->signal_toSameThread.connect(this,&TaskAdapter::applyButton);
		rebroadcastManager->getRebroadcastVoid("saveCurrentButton")->signal_toSameThread.connect(this,&TaskAdapter::saveButton);
		rebroadcastManager->getRebroadcastVoid("previewBlankButton")->signal_toSameThread.connect(this,&TaskAdapter::previewBlankButton);
		rebroadcastManager->getRebroadcastVoid("previewFileButton")->signal_toSameThread.connect(this,&TaskAdapter::previewFileButton);
		rebroadcastManager->getRebroadcastVoid("previewCurrentButton")->signal_toSameThread.connect(this,&TaskAdapter::previewCurrentButton);
		rebroadcastManager->getRebroadcastString("configFileLineEdit")->signal_toSameThread.connect(this,&TaskAdapter::saveFileText);
		rebroadcastManager->getRebroadcastString("previewStringLineEdit")->signal_toSameThread.connect(this,&TaskAdapter::previewText);
		
		Rebroadcaster<RendererSceneInfo>* r = dynamic_cast<Rebroadcaster<RendererSceneInfo>*>
			(rebroadcastManager->getRebroadcast_base("RendererInfo"));
		if(r) r->signal_toSameThread.connect(this,&TaskAdapter::rendererTargetChanged);
		SceneRootInfo sri1;
		SceneRootInfo sri2;
		sri1.name = "Cam3";
		sri2.name = "Cam4";
		sceneRoots.push_back(sri1);
		sceneRoots.push_back(sri2);
		setupSceneRootListInGui();
		//find renderers
		std::vector<unsigned int> services = pAPI->getServiceIDList();
		for(std::vector<unsigned int>::iterator iter = services.begin();iter!=services.end();++iter)
		{
			addPotentialRenderer(*iter);
		}
		pAPI->addEventObserver(NerveAPI::SERVICE_AVAILABLE, serviceAvailableObserver);
		pAPI->addEventObserver(NerveAPI::SERVICE_REMOVED, serviceUnavailableObserver);

		//adding a service which allows input of x,y,z doubles
		pAPI->exposeService(&serviceFactory);
	}
	~TaskAdapter()
	{
		pAPI->hideService(&serviceFactory);
		for(std::list<RendererInfo>::iterator iter=renderers.begin();iter!=renderers.end();++iter)
		{
			pAPI->unbindService(iter->binding);
		}
		while(pAPI->getNumExposedUIs()>0)
		{
			delete pAPI->removeUI(0);
		}

		delete task;
		delete rebroadcastManager;
	}
	//void setRunStatus(bool b){runMQ.addMessage(b);}
	//void launchTask(bool b){launchMQ.addMessage(b);}
	void serviceAvailable(unsigned int id)
	{
		addPotentialRenderer(id);
	}
	void serviceUnavailable(unsigned int id)
	{
		removePotentialRenderer(id);
	}
	void launchButton()
	{
		launchStatus = !launchStatus;//toggle status
		if(launchStatus==true)//newly launched
			pAPI->requestCallbackFromMainApplicationThread(1,NerveAPI::CALLBACK_REQUESTS_BLOCKING);//case 1: launchTask();
		else//newly canceled
		{
			cancelTask();
			rebroadcastManager->setValue_double("taskComplete",0);
		}
		rebroadcastManager->setValue_double("taskActive",100*(float)launchStatus);
	}
	void runButton()
	{
		runStatus = !runStatus;//toggle status
		rebroadcastManager->setValue_double("taskRunning",100*(float)runStatus);
	}
	void applyButton()
	{
		loadConfigFileAtLaunch = true;
		loadConfigStringAtLaunch = false;
		if(task_gui) loadCfgFile();
	}
	void saveButton()
	{
		if(task_gui==0) return;
		QString filename = QFileDialog::getSaveFileName(tag, QString("Save as..."),
                            QString(),
                            QString("NERVE config files (*.ncf);;Text files (*.txt)"));
		if(!filename.isEmpty())
		{
			GuiToolsClass::writeConfigFile(task_gui, filename.toStdString());
		}
	}
	void saveFileText(std::string s){configFileText = s;}
	void previewText(std::string s)
	{
		configStringText = s;
		loadConfigFileAtLaunch = false;
		loadConfigStringAtLaunch = true;
		if(task_gui) loadCfgString();
	}
	void previewBlankButton()
	{
		qtQueue.callFunctionoidNonblocking(new PreviewGui(factory,tag,"",false));
	}
	void previewFileButton()
	{
		printf("%s\n",configFileText.c_str());
		qtQueue.callFunctionoidNonblocking(new PreviewGui(factory,tag,configFileText,true));
	}
	void previewCurrentButton()
	{
		std::string str;
		if(task_gui) str = GuiToolsClass::writeConfigQString(task_gui).toStdString();
		qtQueue.callFunctionoidNonblocking(new PreviewGui(factory,tag,str,false));
	}
	void calledFromGuiThread()
	{
		qtQueue.exec();
	}
	void rendererTargetChanged(RendererSceneInfo info)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		std::list<RendererInfo>::iterator iter = renderers.begin();
		for(;iter!=renderers.end();++iter)
		{
			if(iter->provider_id.compare(info.provider)==0 && iter->service_name.compare(info.service)==0)
			{
				updateRenderer(*iter, info.index);
			}
		}
	}
protected:
	//Gui stuff
	TaskAdapterGui* tag;
	RebroadcastManager* rebroadcastManager;
	FunctionQueueQt qtQueue;
	std::string configFileText;
	std::string configStringText;

	//Renderer stuff
	struct RendererInfo
	{
		std::string provider_id;
		std::string service_name;
		ServiceBinding* binding;
		osg::Group* sceneRoot;
		int sceneRootIndex;
		unsigned int serviceID;
		RendererInfo():binding(0),sceneRoot(0),sceneRootIndex(-1){}
	};
	struct SceneRootInfo
	{
		osg::ref_ptr<osg::Group> root;
		int refCount;
		std::string name;
		osg::NodeCallback* nodeCallback;
	};
	CallbackObserver<unsigned int,TaskAdapter> serviceAvailableObserver;
	CallbackObserver<unsigned int,TaskAdapter> serviceUnavailableObserver;
	MessageQueue<unsigned int> serviceAvailableMQ;
	MessageQueue<unsigned int> serviceUnavailableMQ;
	std::list<RendererInfo> renderers;
	std::vector<SceneRootInfo> sceneRoots;
	OpenThreads::Mutex myMutex;

	//Task stuff
	Task* task;
	SettingsGui* task_gui;
	bool launchStatus;
	bool runStatus;
	bool completedStatus;
	bool loadConfigFileAtLaunch;
	bool loadConfigStringAtLaunch;
	
	//Plugin stuff
	NerveAPI* pAPI;
	TaskAdapterFactory* factory;
	
	//Service stuff
	TaskAdapterAPIFactory serviceFactory;
	
	void calledInLoopDuringThreadedMode()
	{
		rebroadcastManager->rebroadcastAll();
		
		
		if(task==0) return;
		if(completedStatus != task->run(runStatus))
		{
			completedStatus = !completedStatus;
			rebroadcastManager->setValue_double("taskComplete",100*(int)completedStatus);
		}
	}
	void callbackFromMainApplicationThread(int call_id)
	{
		switch(call_id)
		{
		case 1:
			launchTask();
			break;
		default:
			break;
		}
	}
	void launchTask()
	{
		task = factory->createTask();
		task_gui = static_cast<SettingsGui*>(factory->createInterfaceObject(0));
		//task_gui = static_cast<SettingsGui*>(pAPI->createAndRegisterUI(0));
		task->acceptSettingsGuiPtr(task_gui);
		//task->acceptWindowEventsTripleBuffer(something...);

		//set up the gui with appropriate renderers and scene root names
		std::vector<std::string> oldSceneRoots;
		for(unsigned int i=0;i<sceneRoots.size();++i)
		{
			oldSceneRoots.push_back(sceneRoots[i].name);
		}
		sceneRoots.clear();
		osg::ref_ptr<osg::Group> tempRoot = task->getSceneRoot();
		for(unsigned int i=0;i<tempRoot->getNumChildren();++i)
		{
			osg::Camera* cam = dynamic_cast<osg::Camera*>(tempRoot->getChild(i));
			if(cam != 0)
			{
				SceneRootInfo info;
				info.name = cam->getName();
				info.root = cam;
				info.refCount = cam->referenceCount();
				info.nodeCallback = cam->getUpdateCallback();
				sceneRoots.push_back(info);
			}
		}
		if(sceneRoots.size()==0)
		{
			SceneRootInfo info;
			info.name = tempRoot->getName();
			info.root = tempRoot;
			info.refCount = tempRoot->referenceCount();
			info.nodeCallback = tempRoot->getUpdateCallback();
			sceneRoots.push_back(info);
		}

		if(sceneRoots.size() != oldSceneRoots.size())
		{
			setupSceneRootListInGui();
			setupRendererListInGui();
			releaseAllRenderers();//previous settings were invalid, reset to blank
		}
		else
		{
			bool allNamesMatch = true;
			for(unsigned int i=0;i<sceneRoots.size();++i)
			{
				if(oldSceneRoots[i].compare(sceneRoots[i].name) != 0)
				{
					allNamesMatch = false;
					break;
				}
			}
			if(!allNamesMatch)
			{
				setupSceneRootListInGui();
				setupRendererListInGui();
				releaseAllRenderers();//previous settings were invalid, reset to blank
			}
		}
		setupAllRenderers();//Renderer list is configured; start rendering if appropriate
		if(loadConfigFileAtLaunch)
		{
			loadCfgFile();
		}
		if(loadConfigStringAtLaunch)
		{
			loadCfgString();
		}
		/*for(unsigned int i=0;i<renderers.size();++i)
		{
			renderers[i]->invoke("renderOSGScene",sceneRoot.get());
		}*/
		pAPI->exposeUI(task_gui);
	}
	void cancelTask()
	{
		QWidget* w = pAPI->removeUI(task_gui);
		factory->cleanUpInterfaceObject(task_gui);
		task_gui = 0;
		
		cancelAllRenderers();
		waitForSceneRootsToClear();
		delete task;
		task = 0;
		
	}
	void loadCfgFile(){qtQueue.callFunctionoidNonblocking(new LoadCFG(task_gui,configFileText,true));}
	void loadCfgString(){qtQueue.callFunctionoidNonblocking(new LoadCFG(task_gui,configStringText,false));}
	void addPotentialRenderer(unsigned int id)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		bool renderFound = false;
		bool removeFound = false;

		std::string provider;
		std::string service;
		pAPI->getServiceInfo(id,provider,service);
		ServiceMetaInfo info(pAPI->getServiceMetaInfo(id));
		for(unsigned int j=0;j<info.getNumInvocables()+1;++j)
		{
			if( info.getInvocable(j).getName().compare("renderOSGScene")==0 &&
				info.getInvocable(j).getNumArgs()==1 &&
				info.getInvocable(j).checkArgType<osg::Group*>(0) ) renderFound = true;
			if( info.getInvocable(j).getName().compare("removeOSGScene")==0 &&
				info.getInvocable(j).getNumArgs()==1 &&
				info.getInvocable(j).checkArgType<osg::Group*>(0) ) removeFound = true;
		}
		if(renderFound && removeFound)
		{
			RendererInfo renderer;
			renderer.provider_id = provider;
			renderer.service_name = service;
			renderer.serviceID = id;
			renderer.binding = 0;
			renderer.sceneRoot = 0;
			renderer.sceneRootIndex = -1;
			renderers.push_back(renderer);
			qtQueue.callFunctionoidNonblocking(new AddRenderer(provider,service,id,sceneRoots.size(),-1,tag));
		}	
	}
	void removePotentialRenderer(unsigned int id)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		std::list<RendererInfo>::iterator iter = renderers.begin();
		
		while(iter!=renderers.end())
		{
			RendererInfo info = *iter;
			if(info.serviceID==id)
			{
				iter = renderers.erase(iter);
				qtQueue.callFunctionoidNonblocking(new RemoveRenderer(id,tag));
			}
			else ++iter;
		}
	}
	void setupRendererListInGui()
	{
				

		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
				

		std::list<RendererInfo>::iterator iter = renderers.begin();
		for(;iter!=renderers.end();++iter)
		{
			RendererInfo r = *iter;
			iter->sceneRootIndex = 0;
			qtQueue.callFunctionoidNonblocking(
				new RemoveRenderer(r.serviceID,tag) );			
			qtQueue.callFunctionoidNonblocking(
				new AddRenderer(r.provider_id,r.service_name,r.serviceID,sceneRoots.size(),r.sceneRootIndex,tag) );
		}
	}
	void setupSceneRootListInGui()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		std::vector<std::string> vec;
		for(unsigned int i=0;i<sceneRoots.size();++i)
		{
			vec.push_back(sceneRoots[i].name);
		}
		qtQueue.callFunctionoidNonblocking(	new SendSceneRootList(vec,tag) ); 
	}
	void setupAllRenderers()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		std::list<RendererInfo>::iterator iter = renderers.begin();
		for(;iter!=renderers.end();++iter)
		{
			updateRenderer(*iter,-3);//-3 lets the rendererinfo instance pick which scene root to render
		}
	}
	void cancelAllRenderers()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		std::list<RendererInfo>::iterator iter = renderers.begin();
		for(;iter!=renderers.end();++iter)
		{
			updateRenderer(*iter,-2);//-2 cancels rendering but does not release service binding
		}
	}
	void releaseAllRenderers()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		std::list<RendererInfo>::iterator iter = renderers.begin();
		for(;iter!=renderers.end();++iter)
		{
			updateRenderer(*iter,-1);//-1 cancels rendering but and releases service binding
		}
	}
	void updateRenderer(RendererInfo& info, int code)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		switch(code)
		{
		case -1:
			if(info.binding != 0 && info.sceneRoot != 0)
			{
				info.binding->invoke("removeOSGScene",info.sceneRoot);
				info.sceneRoot = 0;
			}
			pAPI->unbindService(info.binding);
			info.binding = 0;
			info.sceneRootIndex=-1;
			break;
		case -2:
			if(info.binding != 0 && info.sceneRoot != 0)
			{
				info.binding->invoke("removeOSGScene",info.sceneRoot);
				info.sceneRoot = 0;
			}
			break;
		case -3:
			if(info.binding != 0)
			{
				if(info.sceneRootIndex >= 0 && info.sceneRootIndex < (int)sceneRoots.size())
				{					
					info.sceneRoot = sceneRoots.at(info.sceneRootIndex).root;
					info.binding->invoke("renderOSGScene",info.sceneRoot);
				}
			}
			break;
		default:
			info.sceneRootIndex = code;
			if(info.binding==0) info.binding=pAPI->bindService(info.serviceID);
			if(info.binding==0) break;
			if(info.sceneRoot) info.binding->invoke("removeOSGScene",info.sceneRoot);
			if(info.sceneRootIndex >= 0 && info.sceneRootIndex < (int)sceneRoots.size())
			{
				info.sceneRoot = sceneRoots.at(info.sceneRootIndex).root;
				info.binding->invoke("renderOSGScene",info.sceneRoot);
			}
		}
		
	}
	void waitForSceneRootsToClear()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		for(unsigned int i=0;i<sceneRoots.size();++i)
		{
			osg::Group* sceneRoot = sceneRoots[i].root;
			int baseRefCount = sceneRoots[i].refCount;
			//printf("Checking for scene root %s to clear: ",sceneRoots[i].name.c_str());
			while(sceneRoot->referenceCount() > baseRefCount) printf(".");
			//printf("ok\n");
			sceneRoots[i].root = 0;
		}
	}
};


