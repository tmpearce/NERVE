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

	PluginInfo(std::string filename, std::string dir):
		dll_filename(filename),
		dll_directory(dir),
		hinst(0),
		num_references(0),
		loaded(false),
		allow_runtime_unloading(true)
		{}
	std::string dll_filename;
	std::string dll_directory;
	std::string getFullPath(){return dll_directory + dll_filename;}
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
	PluginRegistry():taskRegistry(0)
	{
		taskRegistry = TaskRegistry(this);
		HMODULE hModule = LoadLibraryEx(L"Kernel32.dll",0,0);
		if(!hModule)printf("Failed to load Kernel32.dll\n");
		AddDllDirectory = (ADDDLLDIRECTORY)GetProcAddress(hModule, "AddDllDirectory");
		if(!AddDllDirectory)printf("AddDllDirectory==0\n");
	}
	~PluginRegistry(){}
	std::vector<std::string> getPluginTypes(){return pluginTypes;}
	void discoverPlugins(std::string directory, bool useSubDirs);
	void clearUnloadedPluginsFromInfoList();
	NervePluginFactory* getFactory(std::string id_string);
	void releaseFactory(std::string id_string);
	void setPluginTypes();

	std::vector<std::string> pluginTypes;
	std::list<PluginInfo> pluginInfoList;
	std::list<PluginInfo>::iterator activePluginInfoIter;

	TaskRegistry taskRegistry;//backwards compatibility only
	void discoverTasks();//backwards compatibility only

	typedef void (__cdecl *ADDDLLDIRECTORY)(PCWSTR);
	ADDDLLDIRECTORY AddDllDirectory;

	friend class NerveApplication;
};