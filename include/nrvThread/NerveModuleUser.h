#pragma once
#include "nrvThread\NerveModule.h"

//NerveModuleUser is a base class for aggregating and operating NerveModules
class NerveModuleUser
{
public:
	enum OPERATE_ACTION
	{
		REMOVE_USER,
		DONT_REMOVE_USER
	};
	enum REMOVE_ACTION
	{
		DELETE_USER,
		DONT_DELETE_USER
	};
	NerveModuleUser(REMOVE_ACTION action = DONT_DELETE_USER):
		_removeAction(action),
		_operateAction(DONT_REMOVE_USER),
		_requestsAllowed(true)
	{}
	virtual ~NerveModuleUser(){}
	virtual bool requestAddModule(NerveModule* module) = 0;					/* Asynchronus request to add a module to the input queue - returns true if requests are allowed, false otherwise*/
	virtual bool requestRemoveAllModules(bool operateBeforeRemoving) = 0;   /* Asynchronus request to remove all modules, optionally following a final operation of each - returns true if requests are allowed, false otherwise*/
	virtual void cleanup(bool operateBeforeRemoving) = 0;					/* Synchronusly remove all modules, optionally operating each one more time, and disallow requests before returning  */
	virtual OPERATE_ACTION updateAndOperateAllModules() = 0;				/* Updates the container via insertions, removals, module deletions, etc, and finally operates the updated container and returns an OPERATION_ACTION that specifies whether or not to remove this User from the container from which it was operated */
	
	void			setRemoveAction(REMOVE_ACTION action)				/* Specifies whether this User should remain (DONT_REMOVE_USER) or be removed (REMOVE_USER) from the container following operation */
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modUserMutex);
		_removeAction = action;
	}		
	REMOVE_ACTION	getRemoveAction()							/* Indicates whether this User will remain (DONT_REMOVE_USER) or be removed (REMOVE_USER) from the container following operation */
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modUserMutex);
		return _removeAction;
	}
	void			setOperateAction(OPERATE_ACTION action)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modUserMutex);
		_operateAction = action;
	}
	OPERATE_ACTION	getOperateAction()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modUserMutex);
		return _operateAction;
	}
	void			setAllowRequests(bool allowRequests)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modUserMutex);
		_requestsAllowed = allowRequests;
	}
	bool			getAllowRequests()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modUserMutex);
		return _requestsAllowed;
	}
protected:
	// Mutex
	OpenThreads::Mutex	modUserMutex;
	// Data
	OPERATE_ACTION		_operateAction;
	REMOVE_ACTION		_removeAction;
	bool				_requestsAllowed;
};
