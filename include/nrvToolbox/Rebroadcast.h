//Rebroadcasters manages thread-barrier-crossing signals
//by taking incoming signals, storing the values in a queue,
//and re-broadcasting new signal(s) upon request, so the
//receiving thread can determine when it is safe for the 
//signals to act on it with minimal blocking of the
//sending thread.


#pragma once
#include "nrv/SignalsAndSlots.h"
#include <list>
#include <map>
#include <string>


//forward declarations:
class RebroadcastVoid;
class RebroadcastDouble;
class RebroadcastString;
class RebroadcastData;

class Rebroadcast_base : public UsesSlots_threadsafe
{
public:
	virtual void rebroadcast() = 0;
	virtual Rebroadcast_base* getNew() = 0;
	virtual void requestReciprocalConnection(Rebroadcast_base* rhs) = 0;
	virtual RebroadcastVoid* asVoid(){return 0;}
	virtual RebroadcastDouble* asDouble(){return 0;}
	virtual RebroadcastString* asString(){return 0;}
	virtual ~Rebroadcast_base(){}
	
};
template <class T>
class Rebroadcaster : public Rebroadcast_base
{
public:
	signal1<T>			signal_toSameThread;
	signal1_threadsafe<T> signal_toOtherThreads;
	Rebroadcast_base* getNew(){return new Rebroadcaster<T>();}
	void requestReciprocalConnection(Rebroadcast_base* rhs)
	{
		Rebroadcaster<T>* other = dynamic_cast<Rebroadcaster<T>*>(rhs);
		if(other != NULL)
		{
			// Note: 'other' is the RebroadcastDouble to connect to (intended to be in another thread)
			signal_toOtherThreads.connect(other, &Rebroadcaster<T>::slot_connectToOtherThreads);
			other->signal_toOtherThreads.connect(this, &Rebroadcaster<T>::slot_connectToOtherThreads);
		}
	}
	void slot_connectToOtherThreads(T t)
	{
		Lock x(myLock);
		toRebroadcast.push_back(t);
	}
	void slot_connectToSameThread(T t)
	{		
		signal_toOtherThreads.broadcast(t);
		slot_connectToOtherThreads(t);
	}
	void rebroadcast()
	{
		Lock x(myLock);
		for(std::list<T>::iterator iter = toRebroadcast.begin();iter!=toRebroadcast.end();++iter)
		{
			signal_toSameThread.broadcast((*iter));
		}
		toRebroadcast.clear();
	}
private:
	std::list<T> toRebroadcast;
};





class RebroadcastVoid : public Rebroadcast_base
{
public:
	RebroadcastVoid(std::string str = ""): name(str), numBroadcasts(0){}
	Rebroadcast_base* getNew(){return new RebroadcastVoid();}
	void requestReciprocalConnection(Rebroadcast_base* rhs);
	RebroadcastVoid* asVoid(){return this;}
	signal0				signal_toSameThread;
	signal0_threadsafe	signal_toOtherThreads;

	//slot_connectToOtherThreads is connected to a thread-safe
	//signal, potentially from another thread.  The signal
	//is added to a queue which is then rebroadcast
	//on a non-thread-safe signal by calling rebroadcast()
	void slot_connectToOtherThreads();
	
	//slot_connectToSameThread should only be called by 1 thread
	//this immediately broadcasts a threadsafe signal to
	//subscribers, which can be in any thread
	void slot_connectToSameThread();
	void rebroadcast();
	void setName(std::string str){name = str;}
	std::string getName(){return name;}
protected:
	std::string name;
	unsigned int numBroadcasts;
};

class RebroadcastDouble : public Rebroadcast_base
{
public:
	RebroadcastDouble(std::string str = ""):name(str), var_ptr(NULL){}
	virtual Rebroadcast_base* getNew(){return new RebroadcastDouble();}
	void requestReciprocalConnection(Rebroadcast_base* rhs);
	RebroadcastDouble* asDouble(){return this;}
	void setVariableToUpdate(double* var);
	signal1<double>				signal_toSameThread;
	signal1_threadsafe<double>	signal_toOtherThreads;
	//slot_connectToOtherThreads is connected to a thread-safe
	//signal, potentially from another thread.  The signal
	//is added to a queue which is then rebroadcast
	//on a non-thread-safe signal by calling rebroadcast()
	void slot_connectToOtherThreads(double value);

	//slot_connectToSameThread should only be called from a single thread
	//this immediately broadcasts a threadsafe signal to
	//subscribers, which can be in any thread
	void slot_connectToSameThread(double value);
	void rebroadcast();
protected:
	std::string name;
	std::list<double> signal_list;
	double* var_ptr;
};
class RebroadcastString : public Rebroadcast_base
{
public:
	RebroadcastString(std::string str = ""):name(str){}
	virtual Rebroadcast_base* getNew(){return new RebroadcastString();}
	void requestReciprocalConnection(Rebroadcast_base* rhs);
	RebroadcastString* asString(){return this;}
	signal1<std::string>			signal_toSameThread;
	signal1_threadsafe<std::string>	signal_toOtherThreads;
	//slot_connectToOtherThreads is connected to a thread-safe
	//signal, potentially from another thread.  The signal
	//is added to a queue which is then rebroadcast
	//on a non-thread-safe signal by calling rebroadcast()
	void slot_connectToOtherThreads(std::string str);

	//slot_connectToSameThread should only be called by 1 thread
	//this immediately broadcasts a threadsafe signal to
	//subscribers, which can be in any thread
	void slot_connectToSameThread(std::string str);
	void rebroadcast();
protected:
	std::string name;
	std::list<std::string> signal_list;
};

