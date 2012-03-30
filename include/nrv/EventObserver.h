//Templated Event/Observer implementation
//Intended to allow subjects to notify observers when events occur.
//For example, NerveApplication operates independent of any guis but
//the guis need to be notified when the state of the application changes
//such as plugins being launched, services becoming available.
//Using events/observers, NerveApplication need not know the interface into
//the specific gui which is being used - it is up to the gui to subscribe
//to the events which it cares about.

#pragma once
#include "nrv/SignalsAndSlots.h"

//Observer: abstract base class
class ObserverBase
{
public:
	virtual ~ObserverBase(){}
};
template<class T>
class Observer : public ObserverBase, public UsesSlots_threadsafe
{
public:
	virtual void notify(T) = 0;
};

template<class T>
class Event
{
public:
	void broadcast(T t){_signal.broadcast(t);}
	void addObserver(Observer<T>& o)
	{
		_signal.connect(&o, &Observer<T>::notify);
	}
	void removeObserver(Observer<T>& o)
	{
		_signal.disconnect_slot(&o);
	}
	bool addObserverBase(ObserverBase& o)
	{
		Observer<T>* _o = dynamic_cast<Observer<T>*>(&o);
		if(_o) addObserver(*_o);
		return _o != 0;
	}
	signal1_threadsafe<T>* getSignal(){return &_signal;}
	~Event(){/*printf("Event<%s> dtor\n",typeid(T).name());*/}
private:
	signal1_threadsafe<T> _signal;
};

template<class T, class CallbackObject>
class CallbackObserver : public Observer<T>
{
public:
	typedef void (CallbackObject::*CallbackFunction)(T t);
	CallbackObserver(CallbackObject* obj, CallbackFunction fcn):_cbObj(obj), _cbFcn(fcn){}
	void notify(T t)
	{
		(_cbObj->* _cbFcn)(t);
	}
protected:
	CallbackObject*   _cbObj;
	CallbackFunction _cbFcn;
};

template<class T>
class SimpleObserver : public Observer<T>
{
public:
	void notify(T t)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		_t = t;
	}
	T currentValue()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		return _t;
	}
private:
	OpenThreads::Mutex myMutex;
	T _t;
};