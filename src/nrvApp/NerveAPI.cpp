#define BUILD_NERVEPLUGIN_LIBRARY
#include "nrvApp/NerveAPI.h"
#include "nrvApp/PluginHandler.h"
#include "nrvApp/PluginRegistry.h"

void NerveAPI::registerWithApplication(NervePluginFactory* f, PluginRegistry* r)
{
	r->accept(f);
}
void NerveAPI::useThreadedMode()
{
	_d->useThreadedMode();
}
void NerveAPI::callPluginFromMainThread(NervePluginBase* p, int call_id, CALLBACK_REQUESTS_BLOCKING_STATUS status)
{
	_d->callPluginFromMainThread(p, call_id,status);
}
long long int NerveAPI::getCurrentTime(){return _d->getCurrentTime();}
std::vector<std::string> NerveAPI::getLoadedPluginIDs(){return _d->getLoadedPluginIDs();}
std::vector<std::string> NerveAPI::getAvailableFactoryIDs(){return _d->getAvailableFactoryIDs();}
void NerveAPI::setTakeOwnershipOfCreatedPlugins(bool takeOwnership){return _d->setTakeOwnershipOfCreatedPlugins(takeOwnership);}
void NerveAPI::setWillAcceptChildUIs(bool willAcceptUIs){return _d->setWillAcceptChildUIs(willAcceptUIs);}
std::string NerveAPI::createPlugin(std::string factory_id){return _d->createPlugin(factory_id);}
void NerveAPI::cancelChildPlugin(std::string plugin_id){return _d->cancelChildPlugin(plugin_id);}
void NerveAPI::detachChildPlugin(std::string plugin_id){return _d->detachChildPlugin(plugin_id);}
IPlugin* NerveAPI::bindIPlugin(std::string plugin_id){return _d->bindIPlugin(plugin_id);}
void NerveAPI::unbindIPlugin(IPlugin* p){return _d->unbindIPlugin(p);}
bool NerveAPI::exposeIPlugin(IPlugin* p){return _d->exposeIPlugin(p);}
int NerveAPI::hideIPlugin(){return _d->hideIPlugin();}
int NerveAPI::getNumExposedUIs(){return _d->getNumExposedUIs();}
QWidget* NerveAPI::exposeUI(QWidget* ui,std::string title){return _d->exposeUI(ui,title);}
QWidget* NerveAPI::removeUI(QWidget* ui){return _d->removeUI(ui);}

void NerveAPI::exposeService(ServiceFactory *service)
{
	_d->addService(service);
}
void NerveAPI::hideService(ServiceFactory *service)
{
	_d->removeService(service);
}
ServiceBinding*					NerveAPI::bindService(unsigned int serviceID){return _d->bindService(serviceID);}
void							NerveAPI::unbindService(ServiceBinding* binding){return _d->unbindService(binding);}
NerveAPI::IDList				NerveAPI::getServiceIDList(){return _d->getServiceIDs();}
unsigned int					NerveAPI::getServiceID(std::string providerID, std::string serviceName){return _d->getServiceID(providerID,serviceName);}
void							NerveAPI::getServiceInfo(unsigned int serviceID, std::string& providerID, std::string& serviceName)
{
	return _d->getServiceInfo(serviceID, providerID, serviceName);
}
const ServiceMetaInfo&			NerveAPI::getServiceMetaInfo(unsigned int serviceID){return _d->getServiceMetaInfo(serviceID);}
void NerveAPI::addEventObserver(EVENT_TYPE eventToObserve, ObserverBase& observer)
{
	std::string str;
	switch(eventToObserve)
	{
	case PLUGIN_AVAILABLE: str="PluginAvailable";break;
	case PLUGIN_REMOVED: str="PluginRemoved";break;
	case UI_AVAILABLE: str="UIAvailable";break;
	case UI_REMOVED: str="UIRemoved";break;
	case SERVICE_AVAILABLE: str="ServiceAvailable";break;
	case SERVICE_REMOVED: str="ServiceRemoved";break;
	}
	_d->addEventObserver(str, observer);
}