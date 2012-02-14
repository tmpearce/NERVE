#pragma once

#include <map>
#include <vector>
#include <string>
#include <list>
#include "windows.h"
#include "nrvApp/TaskRegistry.h"


class NervePluginFactory;
class PluginRegistry;
struct PluginInfo
{
	typedef std::map<std::string, NervePluginFactory*> FactoryMap;

	PluginInfo(std::string filename):dll_filename(filename),hinst(0),num_references(0),loaded(false),allow_runtime_unloading(true){}
	std::string dll_filename;
	HINSTANCE hinst;
	bool loaded;
	bool allow_runtime_unloading;
	unsigned int num_references;
	FactoryMap factoryMap;
	
	bool loadLibrary(PluginRegistry* p);
	bool freeLibrary();
	bool addFactory(NervePluginFactory* factory);
	NervePluginFactory* getFactory(std::string key_string);
	void releaseFactory(std::string key_string);
	bool matchID(std::string id_string);
	bool isLoaded(){return loaded;}
	std::vector<std::string> getIDs();
};
class PluginRegistry
{
public:
	void accept(NervePluginFactory * factory);
		
private:
	PluginRegistry():taskRegistry(0){taskRegistry = TaskRegistry(this);}
	~PluginRegistry(){}
	std::vector<std::string> getPluginTypes(){return pluginTypes;}
	void discoverPlugins();
	void clearUnloadedPluginsFromInfoList();
	NervePluginFactory* getFactory(std::string id_string);
	void releaseFactory(std::string id_string);
	void setPluginTypes();

	std::vector<std::string> pluginTypes;
	std::list<PluginInfo> pluginInfoList;
	std::list<PluginInfo>::iterator activePluginInfoIter;

	TaskRegistry taskRegistry;//backwards compatibility only
	void discoverTasks();//backwards compatibility only

	friend class NerveApplication;
};