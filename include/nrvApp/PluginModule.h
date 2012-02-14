#pragma once
#include "nrvThread\NerveModule.h"
#include "nrvApp\NervePluginBase.h"

class PluginModule : public NerveModule
{
public:
	PluginModule(NervePluginBase* plugin):p(plugin)
	{
		setOperateAction(DONT_REMOVE_MODULE);
		setRemoveAction(DELETE_MODULE);
	}
	~PluginModule(){}
protected:
	void moduleOperation(NerveModuleUser*)
	{
		p->calledInLoopDuringThreadedMode();
	}
	NervePluginBase* p;
};