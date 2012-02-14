#pragma once
#include "nrvService\ServiceFactory.h"
#include "nrvApp\ServiceRegistry.h"
#include <map>

struct BoundService
{
	BoundService(ServiceBinding* b,ServiceFactory* f, std::string c, std::string p, unsigned int id)
		:binding(b),factory(f),consumer(c),provider(p),service_id(id){}
	ServiceBinding* binding;
	ServiceFactory* factory;
	std::string consumer;
	std::string provider;
	unsigned int service_id;
};
class ServiceRepository
{
public:
	ServiceRepository(){}
	ServiceBinding* createServiceBinding(ServiceFactory* factory, std::string consumer, std::string provider, unsigned int serviceID)
	{
		ServiceBinding* b = factory->getBinding();
		if(b)
		{
			bindingToStructMap[b] = new BoundService(b,factory,consumer,provider,serviceID);
		}
		return b;
	}
	void endServiceBinding(ServiceBinding* binding)
	{
		if(bindingToStructMap.count(binding) > 0)
		{
			BoundService* b = bindingToStructMap[binding];
			bindingToStructMap.erase(binding);
			b->factory->cleanupBinding(binding);
			delete b;
		}
	}
	std::string getConsumerID(ServiceBinding* binding)
	{
		std::map<ServiceBinding*,BoundService*>::iterator iter = bindingToStructMap.find(binding);
		if(iter == bindingToStructMap.end()) return "";
		return iter->second->consumer;
	}
	std::string getProviderID(ServiceBinding* binding)
	{
		std::map<ServiceBinding*,BoundService*>::iterator iter = bindingToStructMap.find(binding);
		if(iter == bindingToStructMap.end()) return "";
		return iter->second->provider;
	}
	unsigned int getServiceID(ServiceBinding* binding)
	{
		std::map<ServiceBinding*,BoundService*>::iterator iter = bindingToStructMap.find(binding);
		if(iter == bindingToStructMap.end()) return 0;
		return iter->second->service_id;
	}
protected:
	std::map<ServiceBinding*,BoundService*>	bindingToStructMap;//maps binding back to factory for cleanup
	
};