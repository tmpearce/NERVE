#define BUILD_NERVETASK_LEGACY_LIBRARY

#include "nrvApp\TaskRegistry.h"
#include "nrvExe\Task.h"
#include "nrvApp\TaskAdapter.h"
#include "nrvApp\PluginRegistry.h"

void TaskRegistry::accept(Task * t)
{
	TaskAdapterFactory* factory = new TaskAdapterFactory(t);
	pluginRegistry->accept(factory);
}

