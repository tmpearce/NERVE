/*
class NerveApplication:  The backbone of NERVE.  On launch, it finds
plugins, starts threads for graphics windows and executable logic,
and creates the Qt gui for the application.

NerveApplication also coordinates functionality between the threads
that handle user input (i.e. the Qt GUI), any loaded plugins (i.e.
the PluginManager class), and the visualization via OSG (i.e.
the GraphicsWindowManager class).

*/
#pragma once

#ifdef BUILD_NERVEAPPLICATION_LIBRARY
#define NERVEAPPLICATION __declspec(dllexport)
#else
#define NERVEAPPLICATION __declspec(dllimport)
#endif

#include "nrvApp\PluginRegistry.h"
#include "nrvApp\ServiceRegistry.h"
#include "nrvApp\PluginManager.h"
#include "nrvApp\PluginHandler.h"
#include "nrvApp\ServiceRepository.h"
#include "nrv\EventObserver.h"
#include <QElapsedTimer>
class GeneralGui;
class QApplication;
class ServiceBinding;
class ServiceFactory;
class QThread;
class QObject;

struct UIInfo{
	std::string plugin_id;
	std::string ui_title;
	QWidget* ui_ptr;
	UIInfo(QWidget* ui, const std::string& id, const std::string& title = "untitled"):plugin_id(id),ui_title(title),ui_ptr(ui){}
};

class QApplicationExecutor
{
public:
	struct Functionoid {virtual void operator()() = 0;};
	virtual void callFunctionoidBlocking(Functionoid& f) = 0;
	virtual void callFunctionoidNonblocking(Functionoid& f) = 0;
};
class PluginCreator : public QApplicationExecutor::Functionoid{
public:
	PluginCreator(PluginHandler*& hd):h(hd){}
	void operator()();
	PluginHandler*& h;
};
class NerveApplication
{
public:
	//Executable-level functions
	NERVEAPPLICATION NerveApplication();	
	void NERVEAPPLICATION launch();
	NERVEAPPLICATION ~NerveApplication(){}

	enum PLUGIN_OWNERSHIP
	{
		PLUGIN_OWNED_BY_APPLICATION,
		PLUGIN_OWNED_BY_PLUGIN
	};
	std::vector<std::string> refreshPlugins();
	std::vector<std::string> getAvailableFactoryIDs()
	{
		return pluginRegistry.getPluginTypes();
	}
	std::vector<std::string> getLoadedPluginIDs()
	{
		return pluginManager.getLoadedPlugins();
	}
	std::string createPlugin(std::string factory_id, PLUGIN_OWNERSHIP owner, PluginHandler* creator=0)
	{
		NervePluginFactory* factory = pluginRegistry.getFactory(factory_id);
		if(factory==NULL) return std::string("Error creating plugin");
		PluginHandler* handler=new PluginHandler(*this,*factory);
		//PluginCreator pc(handler);
		//postEventToQApplication(&pc,true);
		//if(handler==NULL) return std::string("Error creating plugin");
		if(creator==0){owner=PLUGIN_OWNED_BY_APPLICATION;}//should never happen but if it does make sure plugin can be cancelled from the application
		if(owner==PLUGIN_OWNED_BY_PLUGIN) handler->setOwner(creator);
		std::string id = pluginManager.addPluginHandler(*handler,factory_id);

		if(owner== PLUGIN_OWNED_BY_APPLICATION)
			PluginAvailable.broadcast(id);
			
		
		return id;
	}
	void assumePluginOwnership(PluginHandler* p)
	{
		p->setOwner(0);
		PluginAvailable.broadcast(p->id());
	}
	void destroyPlugin(std::string pluginObject_id)
	{
		printf("destroyPlugin(%s)\n",pluginObject_id.c_str());
		PluginHandler* handler = pluginManager.getHandler(pluginObject_id);
		if(!handler) return;
		if(handler->serviceProviderBindings.size() > 0) 
		{
			printf("Failed to cancel %s: another plugin is using a service provided by this plugin.\n",pluginObject_id.c_str());
			return;
		}
		bool applicationOwnsPlugin = handler->getMyOwnerPtr()==0;
		std::string plugin_id = pluginManager.destroyHandler(pluginObject_id);
		pluginRegistry.releaseFactory(plugin_id);
		if(applicationOwnsPlugin)
		{
			PluginRemoved.broadcast(pluginObject_id);
		}
	}


	PluginHandler* getPluginHandler(std::string id){return pluginManager.getHandler(id);}
	void setQApplicationExecutor(QApplicationExecutor* p_qAppExec)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		qAppExec = p_qAppExec;
	}
	void postEventToQApplication(QApplicationExecutor::Functionoid* f, bool block)
	{
		if(f==0)return;
		if(qAppExec==0) 
		{
			(*f)();
		}
		else if(block)
		{
			qAppExec->callFunctionoidBlocking(*f);
		}
		else
		{
			qAppExec->callFunctionoidNonblocking(*f);
		}
	}
	bool isQObjectInQApplicationThread(QObject* o);

	//API access functions
	long long int getTime(){OpenThreads::ScopedLock<OpenThreads::Mutex> lock(timerMutex); return qtimer.elapsed();}
	void handlerExposeUserInterface(QWidget* ui, std::string id, std::string title="untitled"){UIAvailable.broadcast(UIInfo(ui,id,title));}
	void handlerHideUserInterface(QWidget* ui, std::string id, std::string title="untitled"){UIRemoved.broadcast(UIInfo(ui,id,title));}
	void handlerAcceptOrphanUI(QWidget* ui,std::string id){printf("Warning: Qt User Interface orphaned by plugin %s - cleanup protocol unknown\n",id);}
	void handlerExposeService(std::string provider, ServiceFactory* service)
	{
		unsigned int id;
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);		
			id = serviceRegistry.addService(provider,service);
		}
		ServiceAvailable.broadcast(id);
	}
	void handlerHideService(std::string provider, ServiceFactory* service)
	{
		unsigned int id;
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
			id = serviceRegistry.removeService(provider,service);
		}
		ServiceRemoved.broadcast(id);
	}
	std::vector<unsigned int> getServiceIDs()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		return serviceRegistry.getAvailableServices();
	}
	unsigned int getServiceID(std::string provider, std::string service)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		return serviceRegistry.getServiceID(provider,service);
	}
	void getServiceInfo(unsigned int id, std::string p, std::string s)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		return serviceRegistry.getServiceInfo(id,p,s);
	}
	const ServiceMetaInfo& getServiceMetaInfo(unsigned int id)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		return serviceRegistry.getServiceMetaInfo(id);
	}
	
	ServiceBinding* handlerBindService(std::string consumer, unsigned int id)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		ServiceFactory* f = serviceRegistry.getFactory(id);
		std::string provider = serviceRegistry.getProvider(id);
		if(!f) return 0;
		ServiceBinding* b = serviceRepository.createServiceBinding(f,consumer,provider,id);
		if(!b) return 0;
		PluginHandler* c = pluginManager.getHandler(consumer);
		PluginHandler* p = pluginManager.getHandler(provider);
		c->addServiceConsumerBinding(b);
		p->addServiceProviderBinding(b);
		return b;
	}
	void handlerUnbindService(ServiceBinding* binding)
	{
		cancelServiceBinding(binding);
	}

	void cancelServiceBinding(ServiceBinding* binding)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		if(!binding) return;
		PluginHandler* consumer = pluginManager.getHandler(serviceRepository.getConsumerID(binding));
		PluginHandler* provider = pluginManager.getHandler(serviceRepository.getProviderID(binding));
		consumer->removeServiceConsumerBinding(binding);
		provider->removeServiceProviderBinding(binding);
		serviceRepository.endServiceBinding(binding);
	}

	bool addEventObserver(std::string eventToObserve, ObserverBase& observer)
	{
		if(eventToObserve.compare("PluginAvailable")==0){return PluginAvailable.addObserverBase(observer);}
		if(eventToObserve.compare("PluginRemoved")==0){return PluginRemoved.addObserverBase(observer);}
		if(eventToObserve.compare("UIAvailable")==0){return UIAvailable.addObserverBase(observer);}
		if(eventToObserve.compare("UIRemoved")==0){return UIRemoved.addObserverBase(observer);}
		if(eventToObserve.compare("ServiceAvailable")==0){return ServiceAvailable.addObserverBase(observer);}
		if(eventToObserve.compare("ServiceRemoved")==0){return ServiceRemoved.addObserverBase(observer);}
		printf("Error: NerveApplication::addEventObserver: %s is not a valid Event.\n",eventToObserve.c_str());
		return false;
	}
protected:
	Event<std::string> PluginAvailable;
	Event<std::string> PluginRemoved;
	Event<UIInfo>	   UIAvailable;
	Event<UIInfo>	   UIRemoved;
	Event<unsigned int> ServiceAvailable;
	Event<unsigned int> ServiceRemoved;

	QApplicationExecutor* qAppExec;

	PluginRegistry pluginRegistry;
	ServiceRegistry serviceRegistry;
	ServiceRepository serviceRepository;
	
	PluginManager			pluginManager;

	GeneralGui * gui;
	QApplication* app;

	OpenThreads::Mutex myMutex;
	OpenThreads::Mutex timerMutex;
	QElapsedTimer qtimer;

};

