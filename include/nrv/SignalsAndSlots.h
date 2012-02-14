#pragma once
//A basic signal-slot system to make trial components easy to reuse.
//Inspiration taken from Sarah Thompson's sigslot, but it is much simplified.
//using sigslot directly doesn't work in projects which also use Qt due to #define macros on 
//Qt's end which collide with emit and connect calls. Use signal0_threadsafe or signal1_threadsafe
//along with Rebroadcast classes to use signals and slots across thread barriers.  Not the slickest
//of solutions, but it works just fine.
//To use:
//In your sending class, create a public object of the signal you want to send (e.g. a signal0 or
//a signal1<int> or whatever).  The class you wish to receive the signal must derive from UsesSlots.
//In your target class, create a public function that:
//1) returns void;
//2) takes the same number and type of arguments that the signal sends.
//Then, call signal.connect(PtrToTargetObject, &TargetClass::slotfunction)

#include <set>
#include <list>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>

//forward declaration
class UsesSlots;

class signalbase
{
public:
	virtual void disconnect_slot(UsesSlots* user) = 0;
	virtual void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget) = 0;
};
class UsesSlots
{
public:
	UsesSlots(){}
	UsesSlots(const UsesSlots& rhs)
	{	
		_signals::const_iterator iterStart = rhs._connectedSignals.begin();
		_signals::const_iterator iter = rhs._connectedSignals.begin();
		_signals::const_iterator iterEnd = rhs._connectedSignals.end();
		for(;iter!=iterEnd; ++iter)
		{
			(*iter)->copy_target(&rhs, this);
		}
	}
	virtual void connect_signal(signalbase* sig)
	{
		_connectedSignals.insert(sig);
	}
	virtual void disconnect_signal(signalbase* sig)
	{
		_connectedSignals.erase(sig);
	}
	virtual void disconnect_all()
	{
		_signals::iterator iterStart = _connectedSignals.begin();
		_signals::iterator iter = _connectedSignals.begin();
		_signals::iterator iterEnd = _connectedSignals.end();
		for(;iter!=iterEnd; ++iter)
		{
			(*iter)->disconnect_slot(this);
		}
		_connectedSignals.erase(iterStart, iterEnd);
	}
	virtual ~UsesSlots()
	{
		disconnect_all();
	}
protected:
	typedef std::set<signalbase*> _signals;
	_signals _connectedSignals;
};

/**********************************************************************/
/**Connection base classes, 0-5 arguments******************************/
class connectionbase0
{
public:
	virtual void broadcast() = 0;
	UsesSlots* getTargetObject(){return _target;}
	virtual connectionbase0* clone() = 0;
	virtual connectionbase0* connect_to_new_target(UsesSlots* object)= 0;
protected:
	void setTarget(UsesSlots* target){_target=target;}
	UsesSlots* _target;
};
template <class arg1_type>
class connectionbase1
{
public:
	virtual void broadcast(arg1_type arg1) = 0;
	UsesSlots* getTargetObject(){return _target;}
	virtual connectionbase1<arg1_type>* clone() = 0;
	virtual connectionbase1<arg1_type>* connect_to_new_target(UsesSlots* object)= 0;
protected:
	void setTarget(UsesSlots* target){_target=target;}
	UsesSlots* _target;
};

template <class arg1_type, class arg2_type>
class connectionbase2
{
public:
	virtual void broadcast(arg1_type arg1, arg2_type arg2) = 0;
	UsesSlots* getTargetObject(){return _target;}
	virtual connectionbase2<arg1_type, arg2_type>* clone() = 0;
	virtual connectionbase2<arg1_type, arg2_type>* connect_to_new_target(UsesSlots* object)= 0;
protected:
	void setTarget(UsesSlots* target){_target=target;}
	UsesSlots* _target;
};

template <class arg1_type, class arg2_type, class arg3_type>
class connectionbase3
{
public:
	virtual void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3) = 0;
	UsesSlots* getTargetObject(){return _target;}
	virtual connectionbase3<arg1_type, arg2_type, arg3_type>* clone() = 0;
	virtual connectionbase3<arg1_type, arg2_type, arg3_type>* connect_to_new_target(UsesSlots* object)= 0;
protected:
	void setTarget(UsesSlots* target){_target=target;}
	UsesSlots* _target;
};

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type>
class connectionbase4
{
public:
	virtual void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4) = 0;
	UsesSlots* getTargetObject(){return _target;}
	virtual connectionbase4<arg1_type, arg2_type, arg3_type, arg4_type>* clone() = 0;
	virtual connectionbase4<arg1_type, arg2_type, arg3_type, arg4_type>* connect_to_new_target(UsesSlots* object)= 0;
protected:
	void setTarget(UsesSlots* target){_target=target;}
	UsesSlots* _target;
};

template <class arg1_type, class arg2_type, class arg3_type, class arg4_type, class arg5_type>
class connectionbase5
{
public:
	virtual void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5) = 0;
	UsesSlots* getTargetObject(){return _target;}
	virtual connectionbase5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>* clone() = 0;
	virtual connectionbase5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>* connect_to_new_target(UsesSlots* object)= 0;
protected:
	void setTarget(UsesSlots* target){_target=target;}
	UsesSlots* _target;
};
/**********************************************************************/
/**Connection classes, 0-5 arguments***********************************/
template<class slotted_object>
class connection0 : public connectionbase0
{
public:
	connection0():_slotted_object(NULL), _slot(NULL){}
	connection0(slotted_object* object, void (slotted_object::*slot)() ):_slotted_object(object), _slot(slot){setTarget(object);}
	void broadcast()
	{
		(_slotted_object->*_slot)();
	}
	connectionbase0* clone(){return new connection0<slotted_object>(_slotted_object, _slot);}
	connectionbase0* connect_to_new_target(UsesSlots* object){return new connection0<slotted_object>((slotted_object*)object, _slot);}
protected:
	slotted_object* _slotted_object;
	void (slotted_object::*_slot) ();
};
template<class slotted_object, class arg1_type>
class connection1 : public connectionbase1<arg1_type>
{
public:
	connection1():_slotted_object(NULL), _slot(NULL){}
	connection1(slotted_object* object, void (slotted_object::*slot)(arg1_type) ):_slotted_object(object), _slot(slot){setTarget(object);}
	void broadcast(arg1_type arg1)
	{
		(_slotted_object->*_slot)(arg1);
	}
	connectionbase1* clone(){return new connection1<slotted_object, arg1_type>(_slotted_object, _slot);}
	connectionbase1* connect_to_new_target(UsesSlots* object){return new connection1<slotted_object, arg1_type>((slotted_object*)object, _slot);}
protected:
	slotted_object* _slotted_object;
	void (slotted_object::*_slot) (arg1_type);
};

template<class slotted_object, class arg1_type, class arg2_type>
class connection2 : public connectionbase2<arg1_type, arg2_type>
{
public:
	connection2():_slotted_object(NULL), _slot(NULL){}
	connection2(slotted_object* object, void (slotted_object::*slot)(arg1_type, arg2_type) ):_slotted_object(object), _slot(slot){setTarget(object);}
	void broadcast(arg1_type arg1, arg2_type arg2)
	{
		(_slotted_object->*_slot)(arg1, arg2);
	}
	connectionbase2* clone(){return new connection2<slotted_object, arg1_type, arg2_type>(_slotted_object, _slot);}
	connectionbase2* connect_to_new_target(UsesSlots* object){return new connection2<slotted_object, arg1_type, arg2_type>((slotted_object*)object, _slot);}
protected:
	slotted_object* _slotted_object;
	void (slotted_object::*_slot) (arg1_type, arg2_type);
};

template<class slotted_object, class arg1_type, class arg2_type, class arg3_type>
class connection3 : public connectionbase3<arg1_type, arg2_type, arg3_type>
{
public:
	connection3():_slotted_object(NULL), _slot(NULL){}
	connection3(slotted_object* object, void (slotted_object::*slot)(arg1_type, arg2_type, arg3_type) ):_slotted_object(object), _slot(slot){setTarget(object);}
	void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3)
	{
		(_slotted_object->*_slot)(arg1, arg2, arg3);
	}
	connectionbase3* clone(){return new connection3<slotted_object, arg1_type, arg2_type, arg3_type>(_slotted_object, _slot);}
	connectionbase3* connect_to_new_target(UsesSlots* object){return new connection3<slotted_object, arg1_type, arg2_type, arg3_type>((slotted_object*)object, _slot);}
protected:
	slotted_object* _slotted_object;
	void (slotted_object::*_slot) (arg1_type, arg2_type, arg3_type);
};

template<class slotted_object, class arg1_type, class arg2_type, class arg3_type, class arg4_type>
class connection4 : public connectionbase4<arg1_type, arg2_type, arg3_type, arg4_type>
{
public:
	connection4():_slotted_object(NULL), _slot(NULL){}
	connection4(slotted_object* object, void (slotted_object::*slot)(arg1_type, arg2_type, arg3_type, arg4_type) ):_slotted_object(object), _slot(slot){setTarget(object);}
	void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4)
	{
		(_slotted_object->*_slot)(arg1, arg2, arg3, arg4);
	}
	connectionbase4* clone(){return new connection4<slotted_object, arg1_type, arg2_type, arg3_type, arg4_type>(_slotted_object, _slot);}
	connectionbase4* connect_to_new_target(UsesSlots* object){return new connection4<slotted_object, arg1_type, arg2_type, arg3_type, arg4_type>((slotted_object*)object, _slot);}
protected:
	slotted_object* _slotted_object;
	void (slotted_object::*_slot) (arg1_type, arg2_type, arg3_type, arg4_type);
};

template<class slotted_object, class arg1_type, class arg2_type, class arg3_type, class arg4_type, class arg5_type>
class connection5 : public connectionbase5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>
{
public:
	connection5():_slotted_object(NULL), _slot(NULL){}
	connection5(slotted_object* object, void (slotted_object::*slot)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type) ):_slotted_object(object), _slot(slot){setTarget(object);}
	void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5)
	{
		(_slotted_object->*_slot)(arg1, arg2, arg3, arg4, arg5);
	}
	connectionbase5* clone(){return new connection5<slotted_object, arg1_type, arg2_type, arg3_type, arg4_type>(_slotted_object, _slot);}
	connectionbase5* connect_to_new_target(UsesSlots* object){return new connection5<slotted_object, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>((slotted_object*)object, _slot);}
protected:
	slotted_object* _slotted_object;
	void (slotted_object::*_slot) (arg1_type, arg2_type, arg3_type, arg4_type, arg5_type);
};

/**********************************************************************/
/**Signal classes, 0-5 arguments***************************************/
class signal0 : public signalbase
{
protected:
	typedef std::list<connectionbase0*> _connections;
public:
	signal0(){}
	signal0(const signal0& rhs)
	{
		_connections::const_iterator iter = rhs.myConnections.begin();
		_connections::const_iterator iterEnd = rhs.myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			myConnections.push_back((*iter)->clone());
			(*iter)->getTargetObject()->connect_signal(this);
		}
	}
	virtual ~signal0(){disconnect_all();}
	template<class slotted_object>
	void connect(slotted_object* object, void(slotted_object::*slot)())
	{
		connection0<slotted_object>* conn = new connection0<slotted_object>(object, slot);
		myConnections.push_back(conn);
		object->connect_signal(this);
	}

	void broadcast()
	{
		_connections::iterator iter = myConnections.begin();
		for(;iter!=myConnections.end();++iter)
		{
			(*iter)->broadcast();
		}
	}
	void disconnect_slot(UsesSlots* user)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			if((*iter)->getTargetObject()==user)
			{
				delete *iter;//deletes the connection object
				iter = myConnections.erase(iter);
			}
			else 
			{
				++iter;
			}
		}
	}
	void disconnect_all()
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			(*iter)->getTargetObject()->disconnect_signal(this);
			delete (*iter);
			++iter;
		}
		myConnections.erase(myConnections.begin(), myConnections.end());
	}

	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			if((*iter)->getTargetObject()==oldtarget)
			{
				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
			}
		}
	}
protected:
	_connections myConnections;
};


template <class arg1_type>
class signal1 : public signalbase
{
protected:
	typedef std::list<connectionbase1<arg1_type>* > _connections;
public:
	signal1(){}
	signal1(const signal1& rhs)
	{
		_connections::const_iterator iter = rhs.myConnections.begin();
		_connections::const_iterator iterEnd = rhs.myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			myConnections.push_back((*iter)->clone());
			(*iter)->getTargetObject()->connect_signal(this);
		}
	}
	virtual ~signal1(){disconnect_all();}
	template<class slotted_object>
	void connect(slotted_object* object, void(slotted_object::*slot)(arg1_type))
	{
		connection1<slotted_object, arg1_type>* conn = new connection1<slotted_object, arg1_type>(object, slot);
		myConnections.push_back(conn);
		object->connect_signal(this);
	}
	
	void broadcast(arg1_type arg1)
	{
		_connections::iterator iter = myConnections.begin();
		for(;iter!=myConnections.end();++iter)
		{
			(*iter)->broadcast(arg1);
		}
	}
	void disconnect_slot(UsesSlots* user)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			if((*iter)->getTargetObject()==user)
			{
				delete *iter;//deletes the connection object
				iter = myConnections.erase(iter);
			}
			else 
			{
				++iter;
			}
		}
	}

	void disconnect_all()
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			(*iter)->getTargetObject()->disconnect_signal(this);
			delete (*iter);
			++iter;
		}
		myConnections.erase(myConnections.begin(), myConnections.end());
	}

	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			if((*iter)->getTargetObject()==oldtarget)
			{
				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
			}
		}
	}
protected:
	_connections myConnections;
};

template <class arg1_type, class arg2_type>
class signal2 : public signalbase
{
protected:
	typedef std::list<connectionbase2<arg1_type, arg2_type>* > _connections;
public:
	signal2(){}
	signal2(const signal2& rhs)
	{
		_connections::const_iterator iter = rhs.myConnections.begin();
		_connections::const_iterator iterEnd = rhs.myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			myConnections.push_back((*iter)->clone());
			(*iter)->getTargetObject()->connect_signal(this);
		}
	}
	virtual ~signal2(){disconnect_all();}
	template<class slotted_object>
	void connect(slotted_object* object, void(slotted_object::*slot)(arg1_type, arg2_type))
	{
		connection2<slotted_object, arg1_type, arg2_type>* conn = new connection1<slotted_object, arg1_type, arg2_type>(object, slot);
		myConnections.push_back(conn);
		object->connect_signal(this);
	}
	
	void broadcast(arg1_type arg1, arg2_type arg2)
	{
		_connections::iterator iter = myConnections.begin();
		for(;iter!=myConnections.end();++iter)
		{
			(*iter)->broadcast(arg1, arg2);
		}
	}
	void disconnect_slot(UsesSlots* user)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			if((*iter)->getTargetObject()==user)
			{
				delete *iter;//deletes the connection object
				iter = myConnections.erase(iter);
			}
			else 
			{
				++iter;
			}
		}
	}

	void disconnect_all()
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			(*iter)->getTargetObject()->disconnect_signal(this);
			delete (*iter);
			++iter;
		}
		myConnections.erase(myConnections.begin(), myConnections.end());
	}

	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			if((*iter)->getTargetObject()==oldtarget)
			{
				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
			}
		}
	}
protected:
	_connections myConnections;
};

template <class arg1_type, class arg2_type, class arg3_type>
class signal3 : public signalbase
{
protected:
	typedef std::list<connectionbase3<arg1_type, arg2_type, arg3_type>* > _connections;
public:
	signal3(){}
	signal3(const signal3& rhs)
	{
		_connections::const_iterator iter = rhs.myConnections.begin();
		_connections::const_iterator iterEnd = rhs.myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			myConnections.push_back((*iter)->clone());
			(*iter)->getTargetObject()->connect_signal(this);
		}
	}
	virtual ~signal3(){disconnect_all();}
	template<class slotted_object>
	void connect(slotted_object* object, void(slotted_object::*slot)(arg1_type, arg2_type, arg3_type))
	{
		connection3<slotted_object, arg1_type, arg2_type, arg3_type>* conn = new connection1<slotted_object, arg1_type, arg2_type, arg3_type>(object, slot);
		myConnections.push_back(conn);
		object->connect_signal(this);
	}
	
	void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3)
	{
		_connections::iterator iter = myConnections.begin();
		for(;iter!=myConnections.end();++iter)
		{
			(*iter)->broadcast(arg1, arg2, arg3);
		}
	}
	void disconnect_slot(UsesSlots* user)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			if((*iter)->getTargetObject()==user)
			{
				delete *iter;//deletes the connection object
				iter = myConnections.erase(iter);
			}
			else 
			{
				++iter;
			}
		}
	}

	void disconnect_all()
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			(*iter)->getTargetObject()->disconnect_signal(this);
			delete (*iter);
			++iter;
		}
		myConnections.erase(myConnections.begin(), myConnections.end());
	}

	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			if((*iter)->getTargetObject()==oldtarget)
			{
				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
			}
		}
	}
protected:
	_connections myConnections;
};	


template <class arg1_type, class arg2_type, class arg3_type, class arg4_type>
class signal4 : public signalbase
{
protected:
	typedef std::list<connectionbase4<arg1_type, arg2_type, arg3_type, arg4_type>* > _connections;
public:
	signal4(){}
	signal4(const signal4& rhs)
	{
		_connections::const_iterator iter = rhs.myConnections.begin();
		_connections::const_iterator iterEnd = rhs.myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			myConnections.push_back((*iter)->clone());
			(*iter)->getTargetObject()->connect_signal(this);
		}
	}
	virtual ~signal4(){disconnect_all();}
	template<class slotted_object>
	void connect(slotted_object* object, void(slotted_object::*slot)(arg1_type, arg2_type, arg3_type, arg4_type))
	{
		connection4<slotted_object, arg1_type, arg2_type, arg3_type, arg4_type>* conn = new connection1<slotted_object, arg1_type, arg2_type, arg3_type, arg4_type>(object, slot);
		myConnections.push_back(conn);
		object->connect_signal(this);
	}
	
	void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4)
	{
		_connections::iterator iter = myConnections.begin();
		for(;iter!=myConnections.end();++iter)
		{
			(*iter)->broadcast(arg1, arg2, arg3, arg4);
		}
	}
	void disconnect_slot(UsesSlots* user)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			if((*iter)->getTargetObject()==user)
			{
				delete *iter;//deletes the connection object
				iter = myConnections.erase(iter);
			}
			else 
			{
				++iter;
			}
		}
	}

	void disconnect_all()
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			(*iter)->getTargetObject()->disconnect_signal(this);
			delete (*iter);
			++iter;
		}
		myConnections.erase(myConnections.begin(), myConnections.end());
	}

	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			if((*iter)->getTargetObject()==oldtarget)
			{
				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
			}
		}
	}
protected:
	_connections myConnections;
};	
template <class arg1_type, class arg2_type, class arg3_type, class arg4_type, class arg5_type>
class signal5 : public signalbase
{
protected:
	typedef std::list<connectionbase5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>* > _connections;
public:
	signal5(){}
	signal5(const signal5& rhs)
	{
		_connections::const_iterator iter = rhs.myConnections.begin();
		_connections::const_iterator iterEnd = rhs.myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			myConnections.push_back((*iter)->clone());
			(*iter)->getTargetObject()->connect_signal(this);
		}
	}
	virtual ~signal5(){disconnect_all();}
	template<class slotted_object>
	void connect(slotted_object* object, void(slotted_object::*slot)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type))
	{
		connection5<slotted_object, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>* conn = new connection1<slotted_object, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>(object, slot);
		myConnections.push_back(conn);
		object->connect_signal(this);
	}
	
	void broadcast(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5)
	{
		_connections::iterator iter = myConnections.begin();
		for(;iter!=myConnections.end();++iter)
		{
			(*iter)->broadcast(arg1, arg2, arg3, arg4, arg5);
		}
	}
	void disconnect_slot(UsesSlots* user)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			if((*iter)->getTargetObject()==user)
			{
				delete *iter;//deletes the connection object
				iter = myConnections.erase(iter);
			}
			else 
			{
				++iter;
			}
		}
	}

	void disconnect_all()
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			(*iter)->getTargetObject()->disconnect_signal(this);
			delete (*iter);
			++iter;
		}
		myConnections.erase(myConnections.begin(), myConnections.end());
	}

	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
	{
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			if((*iter)->getTargetObject()==oldtarget)
			{
				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
			}
		}
	}
protected:
	_connections myConnections;
};	


/**********************************************************************/
/**Thread-safe classes ************************************************/
class UsesSlots_threadsafe : public UsesSlots
{
protected:
	typedef OpenThreads::ScopedLock<OpenThreads::Mutex> Lock;
public:
	UsesSlots_threadsafe(){}
	UsesSlots_threadsafe(const UsesSlots_threadsafe& rhs)
	{	
		_signals::const_iterator iterStart = rhs._connectedSignals.begin();
		_signals::const_iterator iter = rhs._connectedSignals.begin();
		_signals::const_iterator iterEnd = rhs._connectedSignals.end();
		for(;iter!=iterEnd; ++iter)
		{
			(*iter)->copy_target(&rhs, this);
		}
	}
	void connect_signal(signalbase* sig)
	{
		Lock x(myLock);
		_connectedSignals.insert(sig);
	}
	void disconnect_signal(signalbase* sig)
	{
		Lock x(myLock);
		_connectedSignals.erase(sig);
	}
	void disconnect_all()
	{
		Lock x(myLock);
		_signals::iterator iterStart = _connectedSignals.begin();
		_signals::iterator iter = _connectedSignals.begin();
		_signals::iterator iterEnd = _connectedSignals.end();
		for(;iter!=iterEnd; ++iter)
		{
			(*iter)->disconnect_slot(this);
		}
		_connectedSignals.erase(iterStart, iterEnd);
	}
	virtual ~UsesSlots_threadsafe()
	{
		disconnect_all();
	}
protected:
	typedef std::set<signalbase*> _signals;
	_signals _connectedSignals;
	OpenThreads::Mutex myLock;
};
class signal0_threadsafe : public signalbase
{
protected:
	typedef std::list<connectionbase0*> _connections;
	typedef OpenThreads::ScopedLock<OpenThreads::Mutex> Lock;
public:
	signal0_threadsafe(){}
	signal0_threadsafe(const signal0_threadsafe& rhs)
	{
		_connections::const_iterator iter = rhs.myConnections.begin();
		_connections::const_iterator iterEnd = rhs.myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			myConnections.push_back((*iter)->clone());
			(*iter)->getTargetObject()->connect_signal(this);
		}
	}
	virtual ~signal0_threadsafe(){disconnect_all();}
	template<class slotted_object>
	void connect(slotted_object* object, void(slotted_object::*slot)())
	{
		Lock x(myLock);
		connection0<slotted_object>* conn = new connection0<slotted_object>(object, slot);
		myConnections.push_back(conn);
		object->connect_signal(this);
	}

	void broadcast()
	{
		Lock x(myLock);
		_connections::iterator iter = myConnections.begin();
		for(;iter!=myConnections.end();++iter)
		{
			(*iter)->broadcast();
		}
	}
	void disconnect_slot(UsesSlots* user)
	{
		Lock x(myLock);
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			if((*iter)->getTargetObject()==user)
			{
				delete *iter;//deletes the connection object
				iter = myConnections.erase(iter);
			}
			else 
			{
				++iter;
			}
		}
	}
	void disconnect_all()
	{
		Lock x(myLock);
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			(*iter)->getTargetObject()->disconnect_signal(this);
			delete (*iter);
			++iter;
		}
		myConnections.erase(myConnections.begin(), myConnections.end());
	}

	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
	{
		Lock x(myLock);
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			if((*iter)->getTargetObject()==oldtarget)
			{
				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
			}
		}
	}
protected:
	_connections myConnections;
	OpenThreads::Mutex myLock;
};
template <class arg1_type>
class signal1_threadsafe : public signalbase
{
protected:
	typedef std::list<connectionbase1<arg1_type>* > _connections;
	typedef OpenThreads::ScopedLock<OpenThreads::Mutex> Lock;
public:
	signal1_threadsafe(){}
	signal1_threadsafe(const signal1_threadsafe& rhs)
	{
		_connections::const_iterator iter = rhs.myConnections.begin();
		_connections::const_iterator iterEnd = rhs.myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			myConnections.push_back((*iter)->clone());
			(*iter)->getTargetObject()->connect_signal(this);
		}
	}
	virtual ~signal1_threadsafe(){disconnect_all();}
	template<class slotted_object>
	void connect(slotted_object* object, void(slotted_object::*slot)(arg1_type))
	{
		Lock x(myLock);
		connection1<slotted_object, arg1_type>* conn = new connection1<slotted_object, arg1_type>(object, slot);
		myConnections.push_back(conn);
		object->connect_signal(this);
	}
	void broadcast(arg1_type arg1)
	{
		Lock x(myLock);
		_connections::iterator iter = myConnections.begin();
		for(;iter!=myConnections.end();++iter)
		{
			(*iter)->broadcast(arg1);
		}
	}
	void disconnect_slot(UsesSlots* user)
	{
		Lock x(myLock);
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			if((*iter)->getTargetObject()==user)
			{
				delete *iter;//deletes the connection object
				iter = myConnections.erase(iter);
			}
			else 
			{
				++iter;
			}
		}
	}
	void disconnect_all()
	{
		Lock x(myLock);
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		while(iter != iterEnd)
		{
			(*iter)->getTargetObject()->disconnect_signal(this);
			delete (*iter);
			++iter;
		}
		myConnections.erase(myConnections.begin(), myConnections.end());
	}

	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
	{
		Lock x(myLock);
		_connections::iterator iter = myConnections.begin();
		_connections::iterator iterEnd = myConnections.end();
		for(;iter != iterEnd;++iter)
		{
			if((*iter)->getTargetObject()==oldtarget)
			{
				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
			}
		}
	}	
protected:
	_connections myConnections;
	OpenThreads::Mutex myLock;
};


/**********************************************************************/
/**Signal base classes, 0-5 arguments**********************************/

//class signalbase0 : public signalbase
//{
//protected:
//	typedef std::list<connectionbase0*> _connections;
//public:
//	signalbase0(){}
//
//	signalbase0(const signalbase0& rhs)
//	{
//		_connections::const_iterator iter = rhs.myConnections.begin();
//		_connections::const_iterator iterEnd = rhs.myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			myConnections.push_back((*iter)->clone());
//			(*iter)->getTargetObject()->connect_signal(this);
//		}
//	}
//	virtual ~signalbase0(){disconnect_all();}
//	
//	void disconnect_slot(UsesSlots* user)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			if((*iter)->getTargetObject()==user)
//			{
//				delete *iter;//deletes the connection object
//				iter = myConnections.erase(iter);
//			}
//			else 
//			{
//				++iter;
//			}
//		}
//	}
//	void disconnect_all()
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			(*iter)->getTargetObject()->disconnect_signal(this);
//			delete (*iter);
//			++iter;
//		}
//		myConnections.erase(myConnections.begin(), myConnections.end());
//	}
//
//	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			if((*iter)->getTargetObject()==oldtarget)
//			{
//				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
//			}
//		}
//	}
//protected:
//	_connections myConnections;
//};

//template <class arg1_type>
//class signalbase1 : public signalbase
//{
//protected:
//	typedef std::list<connectionbase1<arg1_type>* > _connections;
//public:
//	signalbase1(){}
//	signalbase1(const signalbase1& rhs)
//	{
//		_connections::const_iterator iter = rhs.myConnections.begin();
//		_connections::const_iterator iterEnd = rhs.myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			myConnections.push_back((*iter)->clone());
//			(*iter)->getTargetObject()->connect_signal(this);
//		}
//	}
//	virtual ~signalbase1(){disconnect_all();}
//	
//	void disconnect_slot(UsesSlots* user)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			if((*iter)->getTargetObject()==user)
//			{
//				delete *iter;//deletes the connection object
//				iter = myConnections.erase(iter);
//			}
//			else 
//			{
//				++iter;
//			}
//		}
//	}
//
//	void disconnect_all()
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			(*iter)->getTargetObject()->disconnect_signal(this);
//			delete (*iter);
//			++iter;
//		}
//		myConnections.erase(myConnections.begin(), myConnections.end());
//	}
//
//	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			if((*iter)->getTargetObject()==oldtarget)
//			{
//				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
//			}
//		}
//	}
//protected:
//	_connections myConnections;
//};

//template <class arg1_type, class arg2_type>
//class signalbase2 : public signalbase
//{
//protected:
//	typedef std::list<connectionbase2<arg1_type, arg2_type>* > _connections;
//public:
//	signalbase2(){}
//	signalbase2(const signalbase2& rhs)
//	{
//		_connections::const_iterator iter = rhs.myConnections.begin();
//		_connections::const_iterator iterEnd = rhs.myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			myConnections.push_back((*iter)->clone());
//			(*iter)->getTargetObject()->connect_signal(this);
//		}
//	}
//	virtual ~signalbase2(){disconnect_all();}
//	
//	void disconnect_slot(UsesSlots* user)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			if((*iter)->getTargetObject()==user)
//			{
//				delete *iter;//deletes the connection object
//				iter = myConnections.erase(iter);
//			}
//			else 
//			{
//				++iter;
//			}
//		}
//	}
//
//	void disconnect_all()
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			(*iter)->getTargetObject()->disconnect_signal(this);
//			delete (*iter);
//			++iter;
//		}
//		myConnections.erase(myConnections.begin(), myConnections.end());
//	}
//
//	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			if((*iter)->getTargetObject()==oldtarget)
//			{
//				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
//			}
//		}
//	}
//protected:
//	_connections myConnections;
//};

//template <class arg1_type, class arg2_type, class arg3_type>
//class signalbase3 : public signalbase
//{
//protected:
//	typedef std::list<connectionbase3<arg1_type, arg2_type, arg3_type>* > _connections;
//public:
//	signalbase3(){}
//	signalbase3(const signalbase3& rhs)
//	{
//		_connections::const_iterator iter = rhs.myConnections.begin();
//		_connections::const_iterator iterEnd = rhs.myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			myConnections.push_back((*iter)->clone());
//			(*iter)->getTargetObject()->connect_signal(this);
//		}
//	}
//	virtual ~signalbase3(){disconnect_all();}
//	
//	void disconnect_slot(UsesSlots* user)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			if((*iter)->getTargetObject()==user)
//			{
//				delete *iter;//deletes the connection object
//				iter = myConnections.erase(iter);
//			}
//			else 
//			{
//				++iter;
//			}
//		}
//	}
//
//	void disconnect_all()
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			(*iter)->getTargetObject()->disconnect_signal(this);
//			delete (*iter);
//			++iter;
//		}
//		myConnections.erase(myConnections.begin(), myConnections.end());
//	}
//
//	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			if((*iter)->getTargetObject()==oldtarget)
//			{
//				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
//			}
//		}
//	}
//protected:
//	_connections myConnections;
//};

//template <class arg1_type, class arg2_type, class arg3_type, class arg4_type>
//class signalbase4 : public signalbase
//{
//protected:
//	typedef std::list<connectionbase4<arg1_type, arg2_type, arg3_type, arg4_type>* > _connections;
//public:
//	signalbase4(){}
//	signalbase4(const signalbase4& rhs)
//	{
//		_connections::const_iterator iter = rhs.myConnections.begin();
//		_connections::const_iterator iterEnd = rhs.myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			myConnections.push_back((*iter)->clone());
//			(*iter)->getTargetObject()->connect_signal(this);
//		}
//	}
//	virtual ~signalbase4(){disconnect_all();}
//	
//	void disconnect_slot(UsesSlots* user)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			if((*iter)->getTargetObject()==user)
//			{
//				delete *iter;//deletes the connection object
//				iter = myConnections.erase(iter);
//			}
//			else 
//			{
//				++iter;
//			}
//		}
//	}
//
//	void disconnect_all()
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			(*iter)->getTargetObject()->disconnect_signal(this);
//			delete (*iter);
//			++iter;
//		}
//		myConnections.erase(myConnections.begin(), myConnections.end());
//	}
//
//	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			if((*iter)->getTargetObject()==oldtarget)
//			{
//				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
//			}
//		}
//	}
//protected:
//	_connections myConnections;
//};
//
//template <class arg1_type, class arg2_type, class arg3_type, class arg4_type, class arg5_type>
//class signalbase5 : public signalbase
//{
//protected:
//	typedef std::list<connectionbase5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>* > _connections;
//public:
//	signalbase5(){}
//	signalbase5(const signalbase5& rhs)
//	{
//		_connections::const_iterator iter = rhs.myConnections.begin();
//		_connections::const_iterator iterEnd = rhs.myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			myConnections.push_back((*iter)->clone());
//			(*iter)->getTargetObject()->connect_signal(this);
//		}
//	}
//	virtual ~signalbase5(){disconnect_all();}
//	
//	void disconnect_slot(UsesSlots* user)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			if((*iter)->getTargetObject()==user)
//			{
//				delete *iter;//deletes the connection object
//				iter = myConnections.erase(iter);
//			}
//			else 
//			{
//				++iter;
//			}
//		}
//	}
//
//	void disconnect_all()
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		while(iter != iterEnd)
//		{
//			(*iter)->getTargetObject()->disconnect_signal(this);
//			delete (*iter);
//			++iter;
//		}
//		myConnections.erase(myConnections.begin(), myConnections.end());
//	}
//
//	void copy_target(const UsesSlots* oldtarget, UsesSlots* newtarget)
//	{
//		_connections::iterator iter = myConnections.begin();
//		_connections::iterator iterEnd = myConnections.end();
//		for(;iter != iterEnd;++iter)
//		{
//			if((*iter)->getTargetObject()==oldtarget)
//			{
//				myConnections.push_back((*iter)->connect_to_new_target(newtarget));
//			}
//		}
//	}
//protected:
//	_connections myConnections;
//};