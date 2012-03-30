#include "nrvApp/TaskAdapter.h"


NervePluginBase* TaskAdapterFactory::createPluginObject(NerveAPI* appInterface)
{
	TaskAdapter* taskAdapter = new TaskAdapter(this,appInterface);
	appInterface->useThreadedMode();

	return taskAdapter;
}