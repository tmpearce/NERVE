#include "nrvToolbox/RebroadcastManager.h"


RebroadcastManager* RebroadcastManager::createReciprocallyConnectedManager()
{
	Lock x(myLock);
	RebroadcastManager* manager = new RebroadcastManager();
	for(rb_map::iterator iter = _map.begin();iter!=_map.end();++iter)
	{
		Rebroadcast_base* rb = (*iter).second->getNew();
		rb->requestReciprocalConnection((*iter).second);
		manager->add(rb, (*iter).first);
	}
	return manager;
}
void RebroadcastManager::add(Rebroadcast_base* toAdd, std::string uniqueName)
{
	Lock x(myLock);
	if(_map.insert(std::pair<std::string, Rebroadcast_base*>(uniqueName, toAdd)).second == true)
	{
		if(toAdd->asDouble())
		{
			DoubleHolder* dh = new DoubleHolder();
			toAdd->asDouble()->signal_toSameThread.connect(dh, &DoubleHolder::slot);
			dh->signal.connect(toAdd->asDouble(), &RebroadcastDouble::slot_connectToSameThread);
			doubleMap[uniqueName]=dh;
		}
		else if(toAdd->asString())
		{
			StringHolder* sh = new StringHolder();
			toAdd->asString()->signal_toSameThread.connect(sh, &StringHolder::slot);
			sh->signal.connect(toAdd->asString(), &RebroadcastString::slot_connectToSameThread);
			stringMap[uniqueName]=sh;
		}
	}
	else
	{
		printf("Error - attempted to add duplicate name (%s) to RebroadcastManager\n", uniqueName.c_str());
		system("pause");
		exit(1);
	}
}
void RebroadcastManager::rebroadcastAll()
{
	Lock x(myLock);
	for(rb_map::iterator iter = _map.begin();iter!=_map.end();++iter)
	{
		(*iter).second->rebroadcast();
	}
	signal_broadcastComplete.broadcast();
	signal_broadcastCompleteThreadsafe.broadcast();
}
RebroadcastVoid* RebroadcastManager::getRebroadcastVoid(std::string name)
{
	rb_map::iterator iter = _map.find(name);
	if(iter==_map.end())
	{
		printf("Error: %s not found in RebroadcastManager::getRebroadcastVoid. Press any key to exit.\n",name.c_str());
		system("pause");
		exit(1);
	}
	if(iter->second->asVoid() == NULL)
	{
		printf("Error: %s is not a RebroadcastVoid (in RebroadcastManager::getRebroadcastVoid(name). Press any key to exit.\n",name.c_str());
		system("pause");
		exit(1);
	}
	return iter->second->asVoid();
}
RebroadcastDouble* RebroadcastManager::getRebroadcastDouble(std::string name)
{
	rb_map::iterator iter = _map.find(name);
	if(iter==_map.end())
	{
		printf("Error: %s not found in RebroadcastManager::getRebroadcastDouble. Press any key to exit.\n",name.c_str());
		system("pause");
		exit(1);
	}
	if(iter->second->asDouble() == NULL)
	{
		printf("Error: %s is not a RebroadcastDouble (in RebroadcastManager::getRebroadcastDouble(name). Press any key to exit.\n",name.c_str());
		system("pause");
		exit(1);
	}
	return iter->second->asDouble();
}
RebroadcastString* RebroadcastManager::getRebroadcastString(std::string name)
{
	rb_map::iterator iter = _map.find(name);
	if(iter==_map.end())
	{
		printf("Error: %s not found in RebroadcastManager::getRebroadcastString. Press any key to exit.\n",name.c_str());
		system("pause");
		exit(1);
	}
	if(iter->second->asString() == NULL)
	{
		printf("Error: %s is not a RebroadcastString (in RebroadcastManager::getRebroadcastString(name). Press any key to exit.\n",name.c_str());
		system("pause");
		exit(1);
	}
	return iter->second->asString();
}
Rebroadcast_base*   RebroadcastManager::getRebroadcast_base(std::string name)
{
	rb_map::iterator iter = _map.find(name);
	if(iter==_map.end())
	{
		printf("Error: %s not found in RebroadcastManager::getRebroadcastBase. Press any key to exit.\n",name.c_str());
		system("pause");
		exit(1);
	}
	return iter->second;
}
void RebroadcastManager::setValue_double(std::string name, double value)
{
	Lock x(myLock);
	double_map::iterator iter = doubleMap.find(name);
	if(iter != doubleMap.end())
	{
		(*iter).second->setDouble(value);
	}
	else
	{
		printf("Error: %s is not a double (in RebroadcastManager::setValue_double(name, value)) - exiting\n",name.c_str());
		system("pause");
		exit(1);
	}
}
void RebroadcastManager::setValue_string(std::string name, std::string str)
{
	Lock x(myLock);
	string_map::iterator iter = stringMap.find(name);
	if(iter != stringMap.end())
	{
		(*iter).second->setString(str);
	}
	else
	{
		printf("Error: %s is not a string (in RebroadcastManager::setValue_string(name, str)) - exiting\n",name.c_str());
		system("pause");
		exit(1);
	}
}
double RebroadcastManager::getCurrentValue_double(std::string name)
{
	Lock x(myLock);
	double_map::iterator iter = doubleMap.find(name);
	if(iter != doubleMap.end())
	{
		return (*iter).second->getDouble();
	}
	else
	{
		printf("Error: %s is not a double (in RebroadcastManager::getCurrentValue_double(name)) - exiting\n",name.c_str());
		system("pause");
		exit(1);
		return 0;
	}
}
std::string RebroadcastManager::getCurrentValue_string(std::string name)
{
	Lock x(myLock);
	string_map::iterator iter = stringMap.find(name);
	if(iter != stringMap.end())
	{
		return (*iter).second->getString();
	}
	else
	{
		printf("Error: %s is not a string (in RebroadcastManager::getCurrentValue_string(name)) - exiting\n",name.c_str());
		system("pause");
		exit(1);
		return std::string();
	}
}
RebroadcastManager::doubleList RebroadcastManager::getAllCurrentValues_double()
{
	Lock x(myLock);
	doubleList l;
	double_map::iterator iter = doubleMap.begin();
	for(;iter!=doubleMap.end();++iter)
	{
		l.push_back( std::pair<std::string, double>((*iter).first, (*iter).second->getDouble()));
	}
	return l;
}
RebroadcastManager::stringList RebroadcastManager::getAllCurrentValues_string()
{
	Lock x(myLock);
	stringList l;
	string_map::iterator iter = stringMap.begin();
	for(;iter!=stringMap.end();++iter)
	{
		l.push_back( std::pair<std::string, std::string>((*iter).first, (*iter).second->getString()));
	}
	return l;
}
RebroadcastManager::~RebroadcastManager()
{
	for(rb_map::iterator iter = _map.begin();iter!=_map.end();++iter)
	{
		delete (*iter).second;
	}
	_map.clear();

	for(double_map::iterator iter = doubleMap.begin();iter!=doubleMap.end();++iter)
	{
		delete (*iter).second;
	}
	doubleMap.clear();

	for(string_map::iterator iter = stringMap.begin();iter!=stringMap.end();++iter)
	{
		delete (*iter).second;
	}
	stringMap.clear();
}