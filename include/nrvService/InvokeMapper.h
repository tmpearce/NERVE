#pragma once
#include <map>
#include <vector>
#include "nrvService/Invoker.h"

class InvokeMapper
{
public:
	typedef std::multimap<std::string, InvokerBase*> InvokerMap;
	InvokerBase* insert(std::string funcName, InvokerBase* invoker)
	{
		if(!invoker) return 0;
		map.insert(std::pair<std::string,InvokerBase*>(funcName,invoker));
		return invoker;
	}
	InvokerBase* findInvoker(std::string funcName, const GenericArgList& args)
	{
		int numFuncs = map.count(funcName);
		if(numFuncs == 0) return 0;//no function of that name was added
		else if(numFuncs == 1) return map.find(funcName)->second;//function not overloaded
		else //function is overloaded, find an appropriate match
		{
			std::vector<std::pair<int,InvokerBase*> > validInvokers;
			std::pair<InvokerMap::iterator,InvokerMap::iterator> iters = map.equal_range(funcName);
			for(InvokerMap::iterator iter = iters.first; iter!=iters.second;++iter)
			{
				if(args.getNumArgs() > (unsigned int) iter->second->getMaxArgs() && args.getNumArgs() < (unsigned int) iter->second->getMinArgs() )
				{
					bool testResult = iter->second->testArgs(args);
					if(testResult)
					{
						int numDefaultsUsed = iter->second->getMaxArgs() - args.getNumArgs();
						std::pair<int,InvokerBase*> invokerPair(numDefaultsUsed,iter->second);
						validInvokers.push_back(invokerPair);
					}
				}
			}
			if(validInvokers.size()==0) return 0;//no valid invokers found in the map
			else if(validInvokers.size()==1) return validInvokers[0].second;//only 1 invoker found
			else
			{
				//if multiple invokers are valid, find the minimum number of default args
				int minDefaults = 10;
				bool multipleMinDefaults = false;
				InvokerBase* selectedInvoker = 0;
				for(unsigned int i=0;i<validInvokers.size();++i)
				{
					if(validInvokers[i].first == minDefaults) multipleMinDefaults = true;
					else if(validInvokers[i].first < minDefaults)
					{
						minDefaults=validInvokers[i].first;
						multipleMinDefaults = false;
						selectedInvoker = validInvokers[i].second;
					}
				}
				if(multipleMinDefaults)
				{
					printf("Error in InvokeMapper::mapInvocation:\nCould not resolve overload\n");
					return 0;
				}
				else if(selectedInvoker==0)
				{
					printf("Error in InvokeMapper::mapInvocation:\nFailed to resolve overload\n");
					return 0;
				}
				else
				{
					return selectedInvoker;
				}
			}
		}		
	}
	bool mapInvocation(std::string funcName,const GenericArgList& args)
	{
		InvokerBase* invoker = findInvoker(funcName,args);
		if(invoker) return invoker->invoke(args);
		else return false;
	}
	bool testArgs(std::string funcName,const GenericArgList& args)
	{
		InvokerBase* invoker = findInvoker(funcName,args);
		if(invoker) return invoker->testArgs(args);
		else return false;
	}
	
	const InvokerMap& getInvokerMap(){return map;}
	
	static void addInvokeMapperToMetaInfo(ServiceMetaInfo& metaInfo, InvokeMapper& mapper)
	{
		const InvokeMapper::InvokerMap map = mapper.getInvokerMap();
		InvokeMapper::InvokerMap::const_iterator iter = map.begin();
		while(iter!=map.end())
		{
			metaInfo.pushInvocable(InvokerBase::infoFromInvoker(iter->second,iter->first));
			++iter;
		}
	}
	static ServiceMetaInfo getMetaInfoForInvokeMapper(InvokeMapper& mapper)
	{
		ServiceMetaInfo metaInfo;
		addInvokeMapperToMetaInfo(metaInfo,mapper);
		return metaInfo;
	}
public:
	InvokeMapper(){}
private:
	InvokerMap map;
};

