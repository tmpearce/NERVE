#include "nrvApp\PluginRegistry.h"
#include "nrvApp\NervePluginFactory.h"
#include <QtCore/QString>
#include <sstream>
std::vector<std::pair<std::wstring, std::wstring> >FindFiles(std::wstring directory, std::wstring templateString, bool recurse)
{
	std::vector<std::pair<std::wstring,std::wstring> > files;
	WIN32_FIND_DATA ffd;
	HANDLE hFind;
	if(directory.empty()) directory=TEXT(".");
	directory.append(TEXT("/"));
	LPCWSTR dir = directory.c_str();
	/*wprintf(TEXT("Searching directory %s\n"),dir);*/
	
	//First look for template matches
	hFind=FindFirstFile((directory + templateString).c_str(),&ffd);
	do
	{
		if (hFind == INVALID_HANDLE_VALUE) { /*printf("invalid handle value\n"); */break; }
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
		files.push_back(std::pair<std::wstring,std::wstring>(directory,std::wstring(ffd.cFileName)));
	}while(FindNextFile(hFind,&ffd) != 0);

	//If recurse is true, recurse through subdirectories
	if(recurse)
	{
		hFind=FindFirstFile((directory + TEXT("*")).c_str(),&ffd);
		do
		{
			if (hFind == INVALID_HANDLE_VALUE) { /*printf("invalid handle value\n"); */break; }
			/*wprintf(TEXT("%s\n"),ffd.cFileName);*/
			if (wcscmp(ffd.cFileName,TEXT("."))==0 || wcscmp(ffd.cFileName,TEXT(".."))==0) continue;
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			{
				std::vector<std::pair<std::wstring,std::wstring> > df =
					FindFiles(directory + ffd.cFileName, templateString, recurse);
				files.insert(files.end(),df.begin(),df.end() );
			}
		}while(FindNextFile(hFind,&ffd) != 0);
	}

	return files;
}

typedef void (__cdecl *PLUGIN_DLL_FUNC)(PluginRegistry*);
void loadPlugin(std::wstring filename, std::wstring dir, 
				 std::list<PluginInfo> &pluginInfoList,
				 std::list<PluginInfo>::iterator &activePluginInfoIter,
				 PluginRegistry* registry)
{
	std::wstring file = dir+filename;
	//wprintf(TEXT("%s\n"),file.c_str());
	HINSTANCE hinst= LoadLibrary(file.c_str());
	if(!hinst)
	{
		wprintf(TEXT("Failure in LoadLibrary(%s: error code %i): check for missing dependencies\n"), file.c_str(),GetLastError()); 
	}
	else
	{
		PLUGIN_DLL_FUNC registerPlugin = (PLUGIN_DLL_FUNC)GetProcAddress(hinst, LPCSTR("RegisterNervePlugin"));
		if(registerPlugin)
		{
			QString tempstr = QString::fromStdWString(file);
			std::list<PluginInfo>::iterator iter = pluginInfoList.begin();
			bool shouldskip = false;
			while(iter != pluginInfoList.end())
			{
				if(iter->getFullPath() == tempstr.toStdString() )
				{
					//printf("Plugin %s already loaded; skipping\n",tempstr.toAscii().constData());
					FreeLibrary(hinst);
					shouldskip=true;
					break;
				}
				++iter;
			}
			if(shouldskip) return;

			pluginInfoList.push_back(
				PluginInfo(QString::fromStdWString(filename).toStdString(),
				           QString::fromStdWString(dir).toStdString()));

			activePluginInfoIter = pluginInfoList.end();
			--activePluginInfoIter;
			registerPlugin(registry);
			pluginInfoList.back().hinst = hinst;
			pluginInfoList.back().loaded = true;
			pluginInfoList.back().freeLibrary();
		}
		else
		{
			FreeLibrary(hinst);
		}
	}
}
void PluginRegistry::accept(NervePluginFactory * factory)
{	
	activePluginInfoIter->addFactory(factory);
}

void PluginRegistry::discoverPlugins(std::string directory, bool useSubDirs)
{
	clearUnloadedPluginsFromInfoList();
	std::vector<std::pair<std::wstring,std::wstring> > files =
		FindFiles(QString::fromStdString(directory).toStdWString(), std::wstring(TEXT("*.dll")),useSubDirs);
	
	for(int i=0;i<files.size();++i)
	{
		loadPlugin(files[i].second,files[i].first,pluginInfoList,activePluginInfoIter,this);

	}

	discoverTasks();
	setPluginTypes();
}


typedef void (__cdecl *TASK_DLL_FUNC)(TaskRegistry*);
void PluginRegistry::discoverTasks()
{

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(TEXT("*.dll"), &FindFileData);
	do
	{
		if (hFind == INVALID_HANDLE_VALUE) { break; } 
		else 
		{
			HINSTANCE hinst= LoadLibrary(FindFileData.cFileName);
			if(!hinst)
			{
				wprintf(TEXT("Failure in LoadLibrary(%s: error code %i): check for missing dependencies\n"), FindFileData.cFileName,GetLastError()); 
				continue;
			}
			else
			{
				TASK_DLL_FUNC registerTask = (TASK_DLL_FUNC)GetProcAddress(hinst, LPCSTR("RegisterNerveTask"));
				if(registerTask)
				{
					QString tempstr = QString::fromWCharArray(FindFileData.cFileName);
					std::list<PluginInfo>::iterator iter = pluginInfoList.begin();
					bool shouldskip = false;
					while(iter != pluginInfoList.end())
					{
						if(iter->dll_filename == tempstr.toStdString() )
						{
							FreeLibrary(hinst);
							shouldskip=true;
							break;
						}
						++iter;
					}
					if(shouldskip)continue;

					pluginInfoList.push_back(PluginInfo(tempstr.toStdString(), std::string("./")));
					activePluginInfoIter = pluginInfoList.end();
					--activePluginInfoIter;

					activePluginInfoIter->hinst = hinst;
					activePluginInfoIter->loaded = true;
					activePluginInfoIter->allow_runtime_unloading = false;
				    registerTask(&taskRegistry);
				}
				else
				{
					FreeLibrary(hinst);
				}
			}
		}
	}
	while(FindNextFile(hFind, &FindFileData) != 0);

}


void PluginRegistry::clearUnloadedPluginsFromInfoList()
{
	std::list<PluginInfo>::iterator iter = pluginInfoList.begin();
	while(iter != pluginInfoList.end())
	{
		if(iter->isLoaded() == false)
		{
			iter = pluginInfoList.erase(iter);
		}
		else ++iter;
	}
}
void PluginRegistry::setPluginTypes()
{
	pluginTypes.clear();
	std::list<PluginInfo>::iterator iter = pluginInfoList.begin();
	while(iter != pluginInfoList.end())
	{
		std::vector<std::string> ids = iter->getIDs();
		pluginTypes.insert(pluginTypes.end(),ids.begin(),ids.end());
		++iter;
	}
}
NervePluginFactory* PluginRegistry::getFactory(std::string id)
{
	NervePluginFactory* p_factory = 0;
	std::list<PluginInfo>::iterator iter = pluginInfoList.begin();//find the PluginInfo object that matches this id
	while(iter != pluginInfoList.end())
	{
		if(iter->matchID(id))//found the right PluginInfo object
		{
			activePluginInfoIter = iter;
			if(iter->isLoaded()==false) iter->loadLibrary(this);
			p_factory = iter->getFactory(id);
			break;
		}
		++iter;
	}
	return p_factory;
}
void PluginRegistry::releaseFactory(std::string id)
{
	std::list<PluginInfo>::iterator iter = pluginInfoList.begin();//find the PluginInfo object that matches this id
	while(iter != pluginInfoList.end())
	{
		if(iter->matchID(id))//found the right PluginInfo object
		{
			iter->releaseFactory(id);
			break;
		}
		++iter;
	}
}
bool PluginInfo::loadLibrary(PluginRegistry* p)
{
	if(!loaded)
	{
		QString filename(dll_filename.c_str());
		hinst=LoadLibrary(filename.toStdWString().c_str());
		if(!hinst)
		{
			wprintf(TEXT("Failure in LoadLibrary(%s: error code %i): check for missing dependencies\n"),filename.toStdWString().c_str(),GetLastError()); 
		}
		else
		{
			PLUGIN_DLL_FUNC registerPlugin = (PLUGIN_DLL_FUNC)GetProcAddress(hinst, LPCSTR("RegisterNervePlugin"));
			if(registerPlugin)
			{
				factoryMap.clear();
			    registerPlugin(p);
				loaded=true;
			}
			else
			{
				loaded=false;
				FreeLibrary(hinst);
			}			
		}
	}
	return hinst!=0;
}
bool PluginInfo::freeLibrary()
{
	if(allow_runtime_unloading==false) return false;

	FactoryMap::iterator iter = factoryMap.begin();
	while(iter!=factoryMap.end())
	{
		if(iter->second)
		{
			iter->second->selfCleanup();
			iter->second = 0;
		}
		++iter;
	}
	FreeLibrary(hinst);
	hinst=0;
	loaded=false;
	
	return true;
}
bool PluginInfo::addFactory(NervePluginFactory* p_factory)
{
	std::string name(p_factory->getName());
	name.append(" <").append(dll_filename).append(">");
	if(factoryMap.count(name)==0 || factoryMap[name] == 0)
	{
		factoryMap[name] = p_factory;
		return true;
	}
	else
	{
		printf("Error: plugin %s attempted to add factory for %s; duplicate name found, factory not added.\n",dll_filename.c_str(),name.c_str());
		return false;
	}
}
NervePluginFactory* PluginInfo::getFactory(std::string key_string)
{
	
	if(!loaded)
	{
		printf("Error - Plugin not loaded: %s",dll_filename.c_str());
		return 0;
	}
	if(factoryMap.count(key_string)>0)
	{
		++num_references;		
		return factoryMap[key_string];		
	}
	else
	{
		printf("Error - Plugin not found: %s",key_string.c_str());
		return 0;
	}
}
void PluginInfo::releaseFactory(std::string key_string)
{
	if(!loaded) return;

	--num_references;
	if(num_references<=0)
	{
		freeLibrary();
	}
}
bool PluginInfo::matchID(std::string id)
{
	if(factoryMap.count(id)>0) return true;
	else return false;
}
std::vector<std::string> PluginInfo::getIDs()
{
	std::vector<std::string> ids;
	FactoryMap::iterator iter = factoryMap.begin();
	while(iter!=factoryMap.end())
	{
		ids.push_back(iter->first);
		++iter;
	}
	return ids;
}