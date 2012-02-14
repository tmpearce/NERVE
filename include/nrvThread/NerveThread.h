#pragma once
#include <OpenThreads/Thread>
#include "nrvThread\BasicModuleUser.h"
/*
NerveThread is a simple interface to operate NerveModules in a thread.
It has a BasicModuleUser which holds the added modules.  The modules are
removed upon calling cancel() on the thread, or can be removed earlier via
NerveModule::setOperateAction(OPERATE_ACTION).
*/
class NerveThread : virtual public OpenThreads::Thread
{
public:
	NerveThread(){}
	virtual ~NerveThread(){}
	virtual void cancelCleanup()//called on cancel() (by OpenThreads::Thread)
	{
		moduleUser.requestRemoveAllModules();
		moduleUser.updateAndOperateAllModules();
	}
	virtual void run()
	{
		while(!testCancel())
		{
			moduleUser.updateAndOperateAllModules();
		}
	}
	void addModule(NerveModule& module)
	{
		moduleUser.requestAddModule(&module);
	}
private:
	BasicModuleUser moduleUser;
};


