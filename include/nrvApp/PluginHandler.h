#pragma once
#include "nrvApp\NerveAPI.h"
#include <set>
#include <map>
#include <string>
#include <OpenThreads/Mutex>
#include "nrv/EventObserver.h"

class NerveApplication;
class ServiceFactory;
class QWidget;
class ServiceMetaInfo;
class NerveThread;
class NervePluginBase;



class PluginHandler
{
protected:
	PluginHandler(NerveApplication& app, NervePluginFactory& factory);
	void setID(std::string id){myID = id;}
	void setupAndRun();
	~PluginHandler();

	void callPluginFromMainThread(NervePluginBase* p, int call_id, NerveAPI::CALLBACK_REQUESTS_BLOCKING_STATUS status);
	
	std::vector<std::string>		getLoadedPluginIDs();
	std::vector<std::string>		getAvailableFactoryIDs();
	void							setTakeOwnershipOfCreatedPlugins(bool takeOwnership);//false unless set to true
	void							setWillAcceptChildUIs(bool willAcceptUIs);//false unless set to true
	std::string						createPlugin(std::string factory_id);
	void							cancelChildPlugin(std::string plugin_id);//null operation if this plugin doesn't own the other plugin
	void							detachChildPlugin(std::string plugin_id);
	PluginAPI*						bindPluginAPI(std::string plugin_id);
	void							unbindPluginAPI(PluginAPI* p);
	bool							exposePluginAPI(PluginAPI* p);
	int								hidePluginAPI();
	PluginAPI*						getPluginAPIBinding(std::string plugin_id);
	void							cancelPluginAPIBinding(std::string plugin_id);
	int								getNumExposedUIs();
	QWidget*						exposeUI(QWidget* ui,std::string title);//returns pointer to ui
	QWidget*						removeUI(QWidget* ui = 0);

	void useThreadedMode();
	void addService(ServiceFactory* service);
	void removeService(ServiceFactory* service);
	ServiceBinding*					bindService(unsigned int serviceID);
	void							unbindService(ServiceBinding* binding);
	std::vector<unsigned int>		getServiceIDs();
	unsigned int					getServiceID(std::string providerID, std::string serviceName);
	void							getServiceInfo(unsigned int serviceID, std::string& providerID, std::string& serviceName);
	const ServiceMetaInfo&			getServiceMetaInfo(unsigned int serviceID);
	void addEventObserver(std::string eventToObserve, ObserverBase& observer);

	void addServiceProviderBinding(ServiceBinding* b){serviceProviderBindings.push_back(b);}
	void addServiceConsumerBinding(ServiceBinding* b){serviceConsumerBindings.push_back(b);}
	void removeServiceProviderBinding(ServiceBinding* b){serviceProviderBindings.remove(b);}
	void removeServiceConsumerBinding(ServiceBinding* b){serviceConsumerBindings.remove(b);}

	void setOwner(PluginHandler* owner){myOwnerPluginHandler=owner;}
	void acceptChildUI(QWidget* child_ui,std::string child_id,std::string title);
	NervePluginBase* getMyPluginPtr(){return myPlugin;}
	PluginHandler*   getMyOwnerPtr(){return myOwnerPluginHandler;}
	std::string      id(){return myID;}

	OpenThreads::Mutex		myMutex;
	NerveThread*			myThread;
	NervePluginFactory*		myPluginFactory;
	NervePluginBase*		myPlugin;
	NerveApplication*		nerveApp;
	PluginHandler*			myOwnerPluginHandler;
	std::string				myID;
	std::string             myPluginID;
	
	bool					myThreadIsRunning;
	bool					threadingRequested;
	bool					takeOwnershipOfPluginsICreate;
	bool					willAcceptChildPluginUIs;

	typedef std::set<QWidget*> uiSet;
	uiSet					myUIs;

	typedef std::list<PluginHandler*> PluginList;
	PluginList				myChildPlugins;	

	typedef std::list<ServiceBinding*> ServiceBindingList;
	ServiceBindingList		serviceProviderBindings;
	ServiceBindingList		serviceConsumerBindings;
	

	PluginAPI*              myPluginAPI;
	bool					pluginAPIExposed;
	typedef std::map<std::string,int> PluginAPIUserMap;
	PluginAPIUserMap pluginAPIUserMap;
	typedef std::map<PluginAPI*, std::string> PluginAPIProviderMap;
	PluginAPIProviderMap pluginAPIProviderMap;

	NerveAPI				myAPI;

	friend class PluginCreator;
	friend class NerveApplication;
	friend class PluginManager;
	friend class NerveAPI;
};