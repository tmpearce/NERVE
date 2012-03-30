#include "nrvApp/TaskAdapter.h"


NervePluginBase* TaskAdapterFactory::createPluginObject()
{
	return new TaskAdapter(this);

}