#pragma once
#include "nrvService\NerveService.h"

class ServiceBinding;

class ServiceFactory
{
public:
	//Meta information accessors
	virtual const ServiceMetaInfo&				getServiceMetaInfo() = 0;

	//Service invoker access
	virtual ServiceBinding*				getBinding() = 0;
	virtual void						cleanupBinding(ServiceBinding* binding) = 0;
	virtual ~ServiceFactory(){}
};
