#pragma once
#include "nrvToolbox/Rebroadcast.h"

class RebroadcastManager : public UsesSlots
{
protected:
	class DoubleHolder : public UsesSlots
	{
	public:
		DoubleHolder():myVal(0.0){}
		signal1<double> signal;
		void slot(double value)
		{
			myVal = value;
		}
		double getDouble()
		{
			return myVal;
		}
		void setDouble(double value)
		{
			myVal = value;
			signal.broadcast(myVal);
		}
	protected:
		double myVal;	
	};
	class StringHolder : public UsesSlots
	{
	public:
		StringHolder(){}
		signal1<std::string> signal;
		void slot(std::string str)
		{
			myStr = str;
		}
		std::string getString(){return myStr;}
		void setString(std::string str)
		{
			myStr = str;
			signal.broadcast(myStr);
		}
	protected:
		std::string myStr;

	};
	typedef std::map<std::string, Rebroadcast_base*> rb_map;
	typedef std::map<std::string, DoubleHolder*> double_map;
	typedef std::map<std::string, StringHolder*> string_map;
	typedef OpenThreads::ScopedLock<OpenThreads::Mutex> Lock;
	typedef std::list<std::pair<std::string,double> > doubleList;
	typedef std::list<std::pair<std::string,std::string> > stringList;
	
public:
	RebroadcastManager(){}
	RebroadcastManager* createReciprocallyConnectedManager();
	void add(Rebroadcast_base* toAdd, std::string uniqueName);
	void rebroadcastAll();
	RebroadcastVoid* getRebroadcastVoid(std::string name);
	RebroadcastDouble* getRebroadcastDouble(std::string name);
	RebroadcastString* getRebroadcastString(std::string name);
	Rebroadcast_base* getRebroadcast_base(std::string name);
	void setValue_double(std::string name, double value);
	void setValue_string(std::string name, std::string str);
	double getCurrentValue_double(std::string name);
	std::string getCurrentValue_string(std::string name);
	doubleList getAllCurrentValues_double();
	stringList getAllCurrentValues_string();
	signal0 signal_broadcastComplete;
	signal0_threadsafe signal_broadcastCompleteThreadsafe;
	~RebroadcastManager();
protected:
	rb_map _map;
	double_map doubleMap;
	string_map stringMap;
	OpenThreads::Mutex myLock;

	
};