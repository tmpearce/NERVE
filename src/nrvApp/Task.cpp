/* Definitions file for the abstract base class Task */
#include "nrvExe/Task.h"
class NoSettingsGui;

Task::Task()
{
	taskname = "Unnamed";
	task2gui = NULL;
	gui2task = NULL;
	_windowevents = NULL;
	//_pointerToVREventsMutex = NULL;
	_completedStatus = false;
	_sceneRoot = new osg::Group();
	
}
Task::~Task()
{
	//printf("Requesting deleteLater on settingsGui\n");
	//settingsGui->deleteLater();
}
bool Task::run(bool doTask)//interface function - do not redefine; implement task in logic() function.
{
	logic(doTask);
	return (getCompletedStatus());
}

osg::ref_ptr<osg::Group> Task::getSceneRoot()//interface function - do not redefine.
{
	customTaskSetup(); 
	return _sceneRoot;
}

osg::ref_ptr<osg::Group> Task::cleanupScene()//interface function - do not redefine.
{
	_sceneRoot = new osg::Group();
	return _sceneRoot;
}

SettingsGui * Task::spawnGui(const SettingsGui * current)
{
	return NULL;
}

void Task::acceptSettingsTripleBuffers(TripleBuffer * Task2Gui, TripleBuffer *Gui2Task)//interface function - do not redefine
{
	//This MUST be called before adding the Settings GUI InterfaceDevice/InterfaceThread
	//This is called by TaskHandlerThread after the constructor but before getSceneRoot()
	task2gui = Task2Gui;
	gui2task = Gui2Task;
}
void Task::acceptWindowEventsTripleBuffer(TripleBuffer* windowevents)
{
	_windowevents=windowevents;
}
void Task::acceptSettingsGuiPtr(SettingsGui* sg)
{
	settingsGui = sg;
}