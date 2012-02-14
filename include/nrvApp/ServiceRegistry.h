#pragma once
#include "nrvService\ServiceFactory.h"
#include <map>
#include <string>
//#include "nrvApp/NerveAPI.h"
class ServiceBinding;

class ServiceRegistry
{
	typedef std::pair<std::string,std::string> ServiceIDString;
public:
	ServiceRegistry():uniqueIDCounter(1){}
	unsigned int addService(std::string provider_id, ServiceFactory* service)
	{		
		ServiceIDString idString(provider_id,service->getServiceMetaInfo().getName());
		unsigned int serviceID;
		if(StringToIDMap.count(idString)==0) 
		{
			serviceID = uniqueIDCounter;
			++uniqueIDCounter;
			serviceMap[serviceID] = service;

			IDtoStringMap[serviceID]=idString;
			StringToIDMap[idString]=serviceID;
			printf("ServiceRegistry::addService <%s::%s> is id#%i\n",provider_id.c_str(), service->getServiceMetaInfo().getName().c_str(),serviceID);
		}
		else
		{
			serviceID = StringToIDMap[idString];
			printf("ServiceRegistry::addService failed:<%s::%s> already exists as id#%i\n",provider_id.c_str(), service->getServiceMetaInfo().getName().c_str(),serviceID);
		}
		return serviceID;
	}
	unsigned int removeService(std::string provider_id, ServiceFactory* service)
	{
		printf("ServiceRegistry::removeService <%s::%s>\n",provider_id.c_str(), service->getServiceMetaInfo().getName().c_str());
		ServiceIDString idString(provider_id,service->getServiceMetaInfo().getName());
		unsigned int serviceID = 0;
		if(StringToIDMap.count(idString)>0)
		{
			serviceID = StringToIDMap[idString];
			StringToIDMap.erase(idString);
			IDtoStringMap.erase(serviceID);
			serviceMap.erase(serviceID);
		}
		return serviceID;
	}
	ServiceFactory* getFactory(unsigned int id)
	{
		if(serviceMap.count(id)==0)//No service of this name was found - return null
		{
			printf("Warning: service %i not found (in ServiceRegistry::getFactory)\n",id);
			return NULL;
		}
		ServiceFactory* factory = serviceMap[id];
		return factory;
	}
	std::vector<unsigned int> getAvailableServices()
	{
		std::vector<unsigned int> vec;
		std::map<unsigned int,ServiceFactory*>::iterator iter = serviceMap.begin();
		while(iter!= serviceMap.end())
		{
			vec.push_back(iter->first);
			++iter;
		}
		return vec;
	}
	unsigned int getServiceID(std::string p, std::string s)
	{
		ServiceIDString idString(p,s);
		unsigned int serviceID = 0;
		if(StringToIDMap.count(idString)>0) 
		{
			serviceID=StringToIDMap[idString];
		}
		return serviceID;
	}
	const ServiceMetaInfo& getServiceMetaInfo(unsigned int id)
	{
		ServiceFactory* factory = getFactory(id);
		return factory->getServiceMetaInfo();
	}
	void getServiceInfo(unsigned int id, std::string& providerID, std::string& serviceName)
	{
		if(IDtoStringMap.count(id)>0)
		{
			providerID=IDtoStringMap[id].first;
			serviceName=IDtoStringMap[id].second;
		}
	}
	std::string getProvider(unsigned int id)
	{
		if(IDtoStringMap.count(id)==0) return "";
		return IDtoStringMap[id].first;
	}
protected:
	std::map<ServiceIDString, unsigned int> StringToIDMap;
	std::map<unsigned int, ServiceIDString> IDtoStringMap;
	std::map<unsigned int,ServiceFactory*> serviceMap;//finds ServiceFactory by provider_id,service_name combo
	unsigned int uniqueIDCounter;
};
