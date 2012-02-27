#pragma once

#ifdef BUILD_NERVEAPPLICATION_LIBRARY
#define NERVEPLUGIN __declspec(dllexport)
#else
#define NERVEPLUGIN __declspec(dllimport)
#define NervePluginSystemVersion 1
#endif
#include <vector>
#include <string>

class PluginHandler;
class QWidget;
class ServiceFactory;
class ServiceBinding;
class ObserverBase;
class ServiceMetaInfo;
class PluginRegistry;
class NervePluginFactory;
class NervePluginBase;

class PluginAPI
{
	virtual ~PluginAPI()=0;
};

class NERVEPLUGIN NerveAPI
{	
public:
	static void registerWithApplication(NervePluginFactory*,PluginRegistry*);

	enum CALLBACK_REQUESTS_BLOCKING_STATUS
	{
		CALLBACK_REQUESTS_BLOCKING,
		CALLBACK_REQUESTS_NONBLOCKING
	};
	enum EVENT_TYPE
	{
		PLUGIN_AVAILABLE,
		PLUGIN_REMOVED,
		UI_AVAILABLE,
		UI_REMOVED,
		SERVICE_AVAILABLE,
		SERVICE_REMOVED
	};

	typedef std::vector<std::string> StringList;
	typedef std::vector<unsigned int> IDList;

	//Threading
	void							useThreadedMode();
	void							callPluginFromMainThread(NervePluginBase* p, int call_id, CALLBACK_REQUESTS_BLOCKING_STATUS status);

	//Plugins
	StringList						getLoadedPluginIDs();
	StringList						getAvailableFactoryIDs();
	void							setTakeOwnershipOfCreatedPlugins(bool takeOwnership);//false unless set to true
	void							setWillAcceptChildUIs(bool willAcceptUIs);//false unless set to true
	std::string						createPlugin(std::string factory_id);
	void							cancelChildPlugin(std::string plugin_id);//null operation if this plugin doesn't own the other plugin
	void							detachChildPlugin(std::string plugin_id);
	PluginAPI*						bindPluginAPI(std::string plugin_id);
	void							unbindPluginAPI(PluginAPI* p);
	bool							exposePluginAPI(PluginAPI* p);//returns success - will fail if there is already an API exposed
	int								hidePluginAPI();//returns the number of plugins that haven't released the API - 0 when all have unbound

	//Qt guis
	int								getNumExposedUIs();
	QWidget*						exposeUI(QWidget* ui,std::string title="");//returns pointer to ui
	QWidget*						removeUI(QWidget* ui = 0);//returns pointer to ui. Input arg=NULL(0): return the first ui owned by this plugin.
															  //this can be used to clean up UIs even if the pointers were not stored directly.
	                                                          //Return value of NULL indicates that the UI could not be removed, or there were no
	                                                          //UIs owned by the plugin to return.

	//Services
	void							exposeService(ServiceFactory* factory);
	void							hideService(ServiceFactory* factory);
	ServiceBinding*					bindService(unsigned int serviceID);
	void							unbindService(ServiceBinding* binding);
	IDList							getServiceIDList();
	unsigned int					getServiceID(std::string providerID, std::string serviceName);
	void							getServiceInfo(unsigned int serviceID, std::string& providerID, std::string& serviceName);
	const ServiceMetaInfo&			getServiceMetaInfo(unsigned int serviceID);

	//Application events
	void							addEventObserver(EVENT_TYPE eventToObserve, ObserverBase& observer);

private:
	/*
		Private functions are accessible by friend class PluginHandler
		The application should route calls through the Handler object
	*/

	~NerveAPI(){}//prevent plugins from trying to delete
	NerveAPI(PluginHandler* ph):_d(ph){}//only allow Handler to create
	NerveAPI(const NerveAPI& rhs); //disable copy construction
	NerveAPI& operator=(NerveAPI& rhs); //disable assignment operator
		
	PluginHandler* _d; //pImpl idiom (private implementation)
	friend class PluginHandler;
};
