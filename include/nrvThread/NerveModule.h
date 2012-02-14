#pragma once
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Mutex>
#ifndef NULL
#define NULL 0
#endif
//#include <sstream>

class NerveModuleUser;

//NerveModule is an abstract base class.  Derived classes must implement
//OPERATE_ACTION operate(NerveModuleUser*);
//OPERATE_ACTION enumermates three possible return values:
//DO_NOT_REMOVE indicates that the module should be left in the module queue of the caller
//REMOVE_AND_DELETE requests that the module be removed from the queue and deleted by the caller
//REMOVE_NO_DELETE requests removal from the queue but that ownership of the module is elsewhere
//  May want to add a list of ModuleUsers that a module has been added to so that when the destructor
//  is called, it can automatically remove itself from its associated ModuleUsers in order to avoid
//  accidental memory problems that could crash a plugin
class NerveModule
{
public:
	enum OPERATE_ACTION
	{
		REMOVE_MODULE_IMMEDIATELY,
		REMOVE_MODULE_AFTER_OPERATING,
		REMOVE_MODULE = REMOVE_MODULE_AFTER_OPERATING,//WARNING!!! Must be directly after REMOVE_MODULE_AFTER_OPERATING - only in here for backwards compatibility 5/23/11
		DONT_REMOVE_MODULE		
	};
	enum REMOVE_ACTION
	{
		DELETE_MODULE,
		DONT_DELETE_MODULE
	};
	NerveModule(REMOVE_ACTION action = DONT_DELETE_MODULE) : 
		_parent(NULL),
		_removeAction(action),
		_operateAction(DONT_REMOVE_MODULE)
		{}
	virtual ~NerveModule(){}
	OPERATE_ACTION operate(NerveModuleUser* modUser)
	{
		if(getOperateAction()==REMOVE_MODULE_IMMEDIATELY) return REMOVE_MODULE_IMMEDIATELY;
		moduleOperation(modUser);
		return getOperateAction();
	}
	void setParent(NerveModuleUser * user)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modMutex);
		_parent = user;
	}
	NerveModuleUser* getParent()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modMutex);
		return _parent;
	}
	void setRemoveAction(REMOVE_ACTION action)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modMutex);
		_removeAction = action;
	}
	REMOVE_ACTION getRemoveAction()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modMutex);
		return _removeAction;
	}
	void setOperateAction(OPERATE_ACTION action)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modMutex);
		_operateAction = action;
	}
	OPERATE_ACTION getOperateAction()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modMutex);
		return _operateAction;
	}

protected:
	virtual void moduleOperation(NerveModuleUser*) = 0;
	// Mutexes
	OpenThreads::Mutex	modMutex;
	// Data	
	NerveModuleUser * _parent;
	REMOVE_ACTION	  _removeAction;
	OPERATE_ACTION    _operateAction;
};





