#include "nrvApp\PluginHandler.h"
#include "nrvApp\NerveApplication.h"
#include "nrvApp\PluginModule.h"
#include "nrvApp\NervePluginFactory.h"
#include "nrvThread\NerveThread.h"
#include "nrvService\ServiceFactory.h"
#include "nrvService\NerveService.h"

#include <OpenThreads\ScopedLock>
#include <QtGui/QWidget>
#include <memory>

class ServiceBinding;
class SelfDeletingOneShotCallback : public QApplicationExecutor::Functionoid
{
public:
	static SelfDeletingOneShotCallback* create(unsigned int i, NervePluginBase* b)
	{
		return new SelfDeletingOneShotCallback(i,b);
	}
	void operator()()
	{
		std::auto_ptr<SelfDeletingOneShotCallback> ap(this);//when the auto_ptr goes out of scope (ie the end of the functionoid) the object is deleted
		_b->callbackFromMainApplicationThread(_i);
	}
	
private:
	SelfDeletingOneShotCallback(unsigned int i, NervePluginBase* b):_i(i),_b(b){}
	unsigned int _i;
	NervePluginBase* _b;
	~SelfDeletingOneShotCallback(){}
	friend class std::auto_ptr<SelfDeletingOneShotCallback>;
};

PluginHandler::PluginHandler(NerveApplication& app, NervePluginFactory& factory):
		myPlugin(NULL),
		myPluginFactory(&factory),
		nerveApp(&app),
		myOwnerPluginHandler(0),
		myThreadIsRunning(false),
		takeOwnershipOfPluginsICreate(false),
		willAcceptChildPluginUIs(false),
		myAPI(this),
		myPluginAPI(0),
		pluginAPIExposed(false)
{		
	myThread = new NerveThread();
}
PluginHandler::~PluginHandler()
{
	//start by setting myThreadIsRunning to true(even if it isn't)
	//so a plugin can't try and start threading after this point
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		myThreadIsRunning = true;
	}
	//cancel the thread and wait for it to stop running
	myThread->cancel();
	while(myThread->isRunning()){}	
	//request the factory clean up the plugin
	//NOTE: don't set myThreadIsRunning to false until after 
	//cleanup is complete in case a plugin requests threading again
	//during destruction for some reason.
	myPluginFactory->cleanUpPluginObject(myPlugin,&myAPI);
	myThreadIsRunning = false;//ok to do now (don't have to though)
	if(!myChildPlugins.empty())
	{
		PluginList::iterator iter = myChildPlugins.begin();
		while(iter!=myChildPlugins.end())
		{
			nerveApp->destroyPlugin((*iter)->id());
			++iter;
		}
	}
	if(!myUIs.empty())//ask the application to handle orphan UIs if both the plugin and factory failed to properly remove them
	{
		std::set<QWidget*>::iterator iter = myUIs.begin();
		while(iter!=myUIs.end())
		{
			nerveApp->handlerAcceptOrphanUI(*iter,myID);
			++iter;
		}
		
	}
	while(serviceConsumerBindings.size()>0)
	{
		nerveApp->handlerUnbindService(serviceConsumerBindings.front());
	}
	delete myThread;
}

void PluginHandler::addService(ServiceFactory* service){nerveApp->handlerExposeService(myID,service);}
void PluginHandler::removeService(ServiceFactory* service){nerveApp->handlerHideService(myID,service);}
void PluginHandler::useThreadedMode()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	threadingRequested = true;
	if(myPlugin && !myThreadIsRunning)
	{
		PluginModule * module = new PluginModule(myPlugin);
		myThread->addModule(*module);
		myThreadIsRunning = true;
		myThread->start();
	}
}
void PluginHandler::callPluginFromMainThread(NervePluginBase* p, int call_id, NerveAPI::CALLBACK_REQUESTS_BLOCKING_STATUS status)
{
	switch(status)
	{
	case NerveAPI::CALLBACK_REQUESTS_BLOCKING:
		nerveApp->postEventToQApplication(SelfDeletingOneShotCallback::create(call_id,p),true);
		break;
	case NerveAPI::CALLBACK_REQUESTS_NONBLOCKING://warning: if the plugin is destroyed before the call is executed, it will crash.
		nerveApp->postEventToQApplication(SelfDeletingOneShotCallback::create(call_id,p),false);
		break;
	}
}
std::vector<std::string> PluginHandler::getLoadedPluginIDs()
{
	return nerveApp->getLoadedPluginIDs();
}
std::vector<std::string> PluginHandler::getAvailableFactoryIDs()
{
	return nerveApp->getAvailableFactoryIDs();
}
void PluginHandler::setTakeOwnershipOfCreatedPlugins(bool takeOwnership)
{
	takeOwnershipOfPluginsICreate=takeOwnership;
}
void PluginHandler::setWillAcceptChildUIs(bool willAcceptUIs)
{
	willAcceptChildPluginUIs=willAcceptUIs;
}
std::string	PluginHandler::createPlugin(std::string factory_id)
{
	std::string retval;
	if(takeOwnershipOfPluginsICreate==true)
	{
		retval = nerveApp->createPlugin(factory_id,NerveApplication::PLUGIN_OWNED_BY_PLUGIN,this);
		PluginHandler* handler = nerveApp->getPluginHandler(retval);
		myChildPlugins.push_back(handler);
	}
	else
	{
		retval = nerveApp->createPlugin(factory_id,NerveApplication::PLUGIN_OWNED_BY_APPLICATION,0);
	}
	return retval;
}
void PluginHandler::cancelChildPlugin(std::string plugin_id)
{
	PluginHandler* child = nerveApp->getPluginHandler(plugin_id);
	bool childFound = false;
	PluginList::iterator iter = myChildPlugins.begin();
	while(iter!=myChildPlugins.end())
	{
		if(*iter == child)
		{
			childFound = true;
			break;
		}
		++iter;
	}
	if(childFound)
	{
		myChildPlugins.erase(iter);
		nerveApp->destroyPlugin(plugin_id);
	}
	else
	{
		printf("NerveAPI(%s)::cancelChildPlugin(%s) failed; child not found. Use the Application to cancel the plugin.\n",myID.c_str(),plugin_id.c_str());
	}
}
void PluginHandler::detachChildPlugin(std::string plugin_id)
{
	nerveApp->assumePluginOwnership(nerveApp->getPluginHandler(plugin_id));
}
PluginAPI* PluginHandler::bindPluginAPI(std::string plugin_id)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	PluginHandler* h=nerveApp->getPluginHandler(plugin_id);
	PluginAPI* p = h->getPluginAPIBinding(myID);
	if(p!=0)
	{
		pluginAPIProviderMap[p] = plugin_id;
	}
	return p;
}
void PluginHandler::unbindPluginAPI(PluginAPI* p)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	PluginAPIProviderMap::iterator iter = pluginAPIProviderMap.find(p);
	if(iter != pluginAPIProviderMap.end())
	{
		PluginHandler* h=nerveApp->getPluginHandler(iter->second);
		h->cancelPluginAPIBinding(myID);
	}
	
}
bool PluginHandler::exposePluginAPI(PluginAPI* p)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	if(myPluginAPI!=0 || p==0) return false;
	myPluginAPI = p;
	pluginAPIExposed = true;
	return true;
}
int PluginHandler::hidePluginAPI()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	pluginAPIExposed = false;
	int users = 0;
	for(PluginAPIUserMap::iterator iter = pluginAPIUserMap.begin();iter!=pluginAPIUserMap.end();++iter)
	{
		users = users+iter->second;
	}
	if(users==0) myPluginAPI=0;
	return users;
}
PluginAPI* PluginHandler::getPluginAPIBinding(std::string id)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	if(pluginAPIExposed==false || myPluginAPI==0)return 0;
	if(pluginAPIUserMap.count(id)>0)
	{
		pluginAPIUserMap[id]++;
	}
	else
	{
		pluginAPIUserMap[id]=1;
	}
	return myPluginAPI;
}
void PluginHandler::cancelPluginAPIBinding(std::string id)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	if(pluginAPIUserMap.count(id)==0) return;
	
	PluginAPIUserMap::iterator iter = pluginAPIUserMap.find(id);
	if(iter->second > 1) iter->second = iter->second - 1;
	else pluginAPIUserMap.erase(iter);	

	if(pluginAPIExposed==false && pluginAPIUserMap.size()==0)//map is empty, can set pointer to 0
	{
		myPluginAPI = 0;
	}
}
int PluginHandler::getNumExposedUIs(){return (int) myUIs.size();}
QWidget* PluginHandler::exposeUI(QWidget* ui,std::string title)
{
	myUIs.insert(ui);
	if(myOwnerPluginHandler != 0)
	{
		myOwnerPluginHandler->acceptChildUI(ui,myID,title);
	}
	else
	{
		nerveApp->handlerExposeUserInterface(ui,myID,title);
	}
	return ui;
}
QWidget* PluginHandler::removeUI(QWidget* ui)
{
	uiSet::iterator iter;
	if(ui==0)
	{
		iter=myUIs.begin();
	}
	else
	{
		iter= myUIs.find(ui);
	}
	if(iter!=myUIs.end())
	{
		QWidget* ui_ptr = *iter;
		myUIs.erase(iter);
		nerveApp->handlerHideUserInterface(ui_ptr,myID);
		printf("ui removed\n");
		return ui_ptr;
		
	}
	else
	{
		return 0;
	}
}
void PluginHandler::acceptChildUI(QWidget* child_ui,std::string child_id,std::string title)
{
	if(willAcceptChildPluginUIs==true)
	{
		std::string str(child_id);
		str.append(" - ").append(title);
		child_ui->setWindowTitle(str.c_str());
		myPlugin->acceptChildUI(child_ui);
	}
	else if(myOwnerPluginHandler != 0)
	{
		myOwnerPluginHandler->acceptChildUI(child_ui,child_id,title);
	}
	else
	{
		nerveApp->handlerExposeUserInterface(child_ui,child_id,title);
	}
}
void PluginHandler::setupAndRun()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	myPlugin = myPluginFactory->createPluginObject(&myAPI);
	if(threadingRequested && !myThreadIsRunning) useThreadedMode();
		
}
ServiceBinding*					PluginHandler::bindService(unsigned int serviceID){return nerveApp->handlerBindService(myID,serviceID);}
void							PluginHandler::unbindService(ServiceBinding* binding){return nerveApp->handlerUnbindService(binding);}
std::vector<unsigned int>		PluginHandler::getServiceIDs(){return nerveApp->getServiceIDs();}
unsigned int					PluginHandler::getServiceID(std::string providerID, std::string serviceName){return nerveApp->getServiceID(providerID,serviceName);}
void							PluginHandler::getServiceInfo(unsigned int serviceID, std::string& providerID, std::string& serviceName)
{
	return nerveApp->getServiceInfo(serviceID,providerID,serviceName);
}
const ServiceMetaInfo&			PluginHandler::getServiceMetaInfo(unsigned int serviceID){return nerveApp->getServiceMetaInfo(serviceID);}
void PluginHandler::addEventObserver(std::string eventToObserve, ObserverBase& observer)
{
	nerveApp->addEventObserver(eventToObserve,observer);
}