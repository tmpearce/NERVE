#pragma once

#include "nrvApp\PluginHandler.h"
#include <string>
#include <sstream>
#include <vector>

class PluginManager
{
protected:
	typedef std::map<std::string, PluginHandler*> HandlerMap;
public:
	PluginManager():refreshRequired(false)
	{}
	std::vector<std::string> getLoadedPlugins(){if(refreshRequired){updateLoadedPlugins();} return loadedPlugins;}
	std::string addPluginHandler(PluginHandler& handler, std::string plugin_id)
	{
		std::string handler_id = createUniqueID(plugin_id);
		handlerMap[handler_id] = &handler;
		handler.setID(handler_id);
		handler.myPluginID = plugin_id;		
		loadedPlugins.push_back(handler_id);
		handler.setupAndRun();		
		return handler_id;
	}
	std::string destroyHandler(std::string handler_id)
	{
		HandlerMap::iterator iter = handlerMap.find(handler_id);
		std::string plugin_id;
		if(iter != handlerMap.end())
		{
			plugin_id= iter->second->myPluginID;
			delete (iter->second);
			handlerMap.erase(iter);
		}
		refreshRequired=true;
		return plugin_id;
	}
	PluginHandler* getHandler(std::string id)
	{
		HandlerMap::iterator iter = handlerMap.find(id);
		if(iter==handlerMap.end())return 0;
		return iter->second;
	}
	
	void clearAllPlugins()
	{
		HandlerMap::iterator iter = handlerMap.begin();
		while(iter!=handlerMap.end())
		{
			destroyHandler(iter->first);
			++iter;
		}
	}
	~PluginManager()
	{
		clearAllPlugins();
	}
protected:
	HandlerMap handlerMap;
	std::vector<std::string> loadedPlugins;
	bool refreshRequired;
	void updateLoadedPlugins()
	{
		loadedPlugins.clear();
		HandlerMap::iterator iter = handlerMap.begin();
		while(iter!=handlerMap.end())
		{
			loadedPlugins.push_back(iter->first);
		}
	}
	std::string createUniqueID(std::string name)
	{
		if(handlerMap.count(name)==0)//no task with this name is in the map
		{
			return name;
		}
		else
		{
			int val=1;
			while(true)//find an integer value to append to the name to make it unique... break when a suitable name is acheived
			{
				val++;
				std::ostringstream testName;
				testName<<name<<"("<<val<<")";
				if(handlerMap.count(testName.str())==0)
				{
					return testName.str();
				}
			}
		}
	}
};