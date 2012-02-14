//TaskRegistry holds the factory functions for GUIs and the tasks for each
//loaded task.  Due to how Qt works, the creation of all windows must happen
//from within the thread that has the QApplication.  This effectively means
//that calls to TaskRegistry should be routed through the GUI, and thus the
//registry and the GUI are in the same thread.  Therefore the threadsafe signal
//and slot system using rebroadcasters is not needed for communication to the
//GUI.  However, threadsafe signals are sent from TaskRegistry to other threads
//(i.e. the TaskHandlerThread).

#pragma once

#ifdef BUILD_NERVETASK_LEGACY_LIBRARY
#define NERVETASK_LEGACY __declspec(dllexport)
#else
#define NERVETASK_LEGACY __declspec(dllimport)
#endif

#include <map>
#include <vector>
#include <string>

class Task;
class GraphicsWindowFactory;
class NervePluginFactory;
class PluginRegistry;

class NERVETASK_LEGACY TaskRegistry
{
public:
	TaskRegistry(PluginRegistry* p):pluginRegistry(p){}
	void accept(Task * t);
	void accept(GraphicsWindowFactory * factory);
	
protected:
	PluginRegistry* pluginRegistry;
};