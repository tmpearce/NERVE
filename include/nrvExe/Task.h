/* Header file for abstract base class Task        */
/* All tasks to be run through the NERVE framework */
/* must derive from Task and implement the         */
/* following pure virtual member functions:        */
/* void customTaskSetup()					       */
/* void logic()								       */
/* Task * spawnTask()	    				       */
/* -----------------------					       */
/* Additionally if a settings GUI is desired,	   */
/* override the following virtual member function: */
/* SettingsGui * spawnGui()	 */

#pragma once
#ifndef TASK_H
#define TASK_H

#ifdef BUILD_NERVETASK_LEGACY_LIBRARY
#define NERVETASK_LEGACY __declspec(dllexport)
#else
#define NERVETASK_LEGACY __declspec(dllimport)
#endif

#include <osg/ref_ptr>
#include <osg/Group>

//forward declarations
class SettingsGui;
class GMutex;
class TripleBuffer;


class NERVETASK_LEGACY Task
{
public:
	/* Constructor: sets taskname to "Unnamed".       */
	/* Derived classes should set taskname to the appropriate string in their constructor */
	Task();
	/* Destructor: override if 'new'-created objects must be 'delete'd by the derived class */
	virtual ~Task();
	/* void run() - Interface function, do not redefine.  run() calls logic() which must be               */
	/* defined by derived classes to implement the desired functionality of the class.                    */
	/* run() begins by automatically updating any interface-linked DataContainers (via _InputArray) */
	/* with new data, and updates any output mutexes (via _OutputArray) following logic().                */
	bool run(bool doTask = true);
	/* osg::ref_ptr<osg::Group> getSceneRoot() - Interface function, do not redefine. */
	/* return: ref_ptr to the scene root created by the derived class.                */
	/* This function creates a _sceneRoot, calls user-defined initializeTask(),       */
	/* then calls checkScene() to perform basic checks on the derived classes scene.  */
	/* Finally all output mutexes are set and interface threads are started (provided */
	/* they are handled by the Array classes designed for that purpose).              */
	osg::ref_ptr<osg::Group> getSceneRoot();
	/* osg::ref_ptr<osg::Group> cleanupScene() - Interface function, do not redefine. */
	/* return: ref_ptr to a scene root with default children (added by checkScene()). */
	osg::ref_ptr<osg::Group> cleanupScene();
	/* void acceptSettingsTripleBuffers(TripleBuffer * Task2Gui, TripleBuffer *Gui2Task)*/
	/* - Interface function, do not redefine											*/
	/* Handled automatically by TaskRegistry.                                           */
	void acceptSettingsTripleBuffers(TripleBuffer * Task2Gui, TripleBuffer *Gui2Task);
	/* void acceptWindowEventsTripleBuffer(TripleBuffer * windowevents)					*/
	/* - Interface function, do not redefine											*/
	/* Handled automatically by TaskHandlerThread.                                      */
	void acceptWindowEventsTripleBuffer(TripleBuffer * windowevents);

	/* void acceptSettingsGuiPtr(SettingsGui * sg)*/
	/* - Interface function, do not redefine											*/
	/* Handled automatically by TaskRegistry.                                           */
	virtual void acceptSettingsGuiPtr(SettingsGui * sg);

	/* std::string getName() - Interface function, do not redefine.                                      */
	/* return: taskname, a const char* data member set in derived class constructor (default: "Unnamed") */
	std::string getName(){return taskname;}
	/* Task * spawnTask(const Task * current) = 0; Pure Virtual function    */
	/* Use this pattern for Task * derived::spawnTask(const Task * current) return new DerivedTask();*/
	virtual Task * spawnTask(const Task * current) = 0;
	/* SettingsGui * spawnGui(const SettingsGui * current)                               */
	/* Use this pattern for SettingsGui * DerivedTask::spawnGui(const SettingsGui * current) */
	/*	{
			return new DerivedSettingsGui();
		}																			     */
	virtual SettingsGui * spawnGui(const SettingsGui * current);
		
protected:
	virtual void logic(bool doTask) = 0;
	virtual void customTaskSetup() = 0;
	virtual void customCleanupSteps(){};
	void setCompletedStatus(bool status){_completedStatus = status;}
	bool getCompletedStatus(){return _completedStatus;}

	/*Data members*/
	const char *taskname;
	bool _completedStatus;

	osg::ref_ptr<osg::Group> _sceneRoot;
	
	TripleBuffer * task2gui, *gui2task;
	TripleBuffer *_windowevents;
	SettingsGui * settingsGui;
	//GMutex * _pointerToVREventsMutex;

};

#endif