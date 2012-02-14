#pragma once
#include "nrvThread/NerveModuleUser.h"
#include <list>
#include <map>

class BasicModuleUser : virtual public NerveModuleUser
{
public:
	// Type Definitions
	typedef std::list<NerveModule*>							ModuleList;

	// Public methods
	BasicModuleUser(REMOVE_ACTION action = DONT_DELETE_USER)
		:	NerveModuleUser(action),
			_removeAllFlag(false),
			_operateBeforeRemovingAll(false) {}
	virtual ~BasicModuleUser()
	{
		setAllowRequests(false);
		insertAddedModules();
		removeAllModules();
	}
	virtual bool requestAddModule(NerveModule * module)
	{
		if (getAllowRequests()==false) return false;
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(addListMutex);
		_addList.push_back(module);
		return true;
	}
	virtual bool requestRemoveAllModules(bool operateBeforeRemoving = false)
	{
		if (getAllowRequests()==false) return false;
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(removeAllMutex);
		_removeAllFlag = true;
		_operateBeforeRemovingAll = operateBeforeRemoving;
		return true;
	}
	virtual void cleanup(bool operateBeforeRemoving = false)
	{
		setAllowRequests(false);
		requestRemoveAllModules(operateBeforeRemoving);
		insertAddedModules();
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(removeAllMutex);
			if(_removeAllFlag)
			{
				if(_operateBeforeRemovingAll)
				{					
					operateAllModules();
				}
				removeAllModules();
			}
		}
	}
	virtual OPERATE_ACTION updateAndOperateAllModules()
	{
		insertAddedModules();
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(removeAllMutex);
			if(_removeAllFlag)
			{
				if(_operateBeforeRemovingAll)
				{					
					operateAllModules();
				}
				removeAllModules();
			}
		}
		operateAllModules();
		return getOperateAction();
	}	
	virtual bool requestRemoveModuleBeforeOperating(NerveModule* module)//equivalent to NerveModule::setOperateAction(REMOVE_MODULE_IMMEDIATELY)
	{
		if (getAllowRequests()==false) return false;
		module->setOperateAction(NerveModule::REMOVE_MODULE_IMMEDIATELY);
		return true;
	}
protected:
	virtual void removeAllModules()//removeAllMutex should be locked before calling this function
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modListMutex);
		ModuleList::iterator modIter = _moduleList.begin();
		while(!_moduleList.empty())
		{
			removeModuleAndIncrement(modIter);
		}
		_removeAllFlag = false;
	}
	virtual void removeModuleAndIncrement(ModuleList::iterator & iter)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modListMutex);
		if ( (*iter)->getRemoveAction() == NerveModule::DELETE_MODULE )
			delete (*iter);
		iter = _moduleList.erase(iter);		// Remove and make iter point to next
	}
	virtual void insertAddedModules()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(addListMutex);
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock2(modListMutex);
		if(!_addList.empty())
		{
			for(ModuleList::iterator addIter = _addList.begin(); addIter != _addList.end(); ++addIter)
			{
				// Add new to module list
				_moduleList.push_back(*addIter);
			}
			_addList.clear();
		}
	}
	virtual void operateAllModules()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(modListMutex);
		ModuleList::iterator iter = _moduleList.begin();
		while(iter != _moduleList.end())
		{		
			NerveModule::OPERATE_ACTION action;	
			if( (*iter)->getOperateAction() == NerveModule::REMOVE_MODULE_IMMEDIATELY )
			{
				removeModuleAndIncrement(iter);
				return;
			}
			else
			{
				action = (*iter)->operate(this);
			}
			
			if(action==NerveModule::REMOVE_MODULE_AFTER_OPERATING || action==NerveModule::REMOVE_MODULE_IMMEDIATELY)
			{
				removeModuleAndIncrement(iter);
			}
			else
			{
				++iter;
			}
		}
	}
	// Mutexes
	OpenThreads::Mutex	addListMutex;
	OpenThreads::Mutex  removeAllMutex;
	OpenThreads::Mutex  modListMutex;
	// Lists		
	ModuleList		_addList;
	ModuleList		_moduleList;
	// Flags
	bool _operateBeforeRemovingAll;
	bool _removeAllFlag;
};