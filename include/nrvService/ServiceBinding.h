#pragma once
#include <string>
#include "nrvService\GenericArgument.h"

class ServiceBinding
{
	class DirectInvoker
	{
	public:
		virtual bool invoke() = 0;
	};
public:
	
	virtual bool		invoke(std::string invocableName, GenericArgList& argList) = 0;
	
	
	inline virtual bool	invoke(std::string invocableName,
									GenericArg a0 = GenericArg(),
									GenericArg a1 = GenericArg(),
									GenericArg a2 = GenericArg(),
									GenericArg a3 = GenericArg(),
									GenericArg a4 = GenericArg(),
									GenericArg a5 = GenericArg(),
									GenericArg a6 = GenericArg(),
									GenericArg a7 = GenericArg(),
									GenericArg a8 = GenericArg(),
									GenericArg a9 = GenericArg() )
	{
		GenericArgList argList;
		if(a0.valid()) argList.pushArg(a0);
		if(a1.valid()) argList.pushArg(a1);
		if(a2.valid()) argList.pushArg(a2);
		if(a3.valid()) argList.pushArg(a3);
		if(a4.valid()) argList.pushArg(a4);
		if(a5.valid()) argList.pushArg(a5);
		if(a6.valid()) argList.pushArg(a6);
		if(a7.valid()) argList.pushArg(a7);
		if(a8.valid()) argList.pushArg(a8);
		if(a9.valid()) argList.pushArg(a9);
		return invoke(invocableName,argList);
	}
	virtual bool		invoke(std::string invocableName, const GenericArgList& argList)
	{
		GenericArgList newargs = argList;
		return invoke(invocableName,newargs);
	}
	virtual				~ServiceBinding(){}
	virtual bool		testArgumentList(std::string invocableName, GenericArgList const& argList) = 0;
	virtual DirectInvoker*	getDirectInvoker(std::string invocableName, GenericArgList const& argList){return NULL;}
};