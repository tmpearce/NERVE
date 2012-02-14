#include "nrvToolbox/Rebroadcast.h"


void RebroadcastVoid::requestReciprocalConnection(Rebroadcast_base* rhs)
{
	RebroadcastVoid* other = rhs->asVoid();
	if(other != NULL)
	{
		// Note: 'other' is the RebroadcastVoid to connect to (intended to be in another thread)
		signal_toOtherThreads.connect(other, &RebroadcastVoid::slot_connectToOtherThreads);
		other->signal_toOtherThreads.connect(this, &RebroadcastVoid::slot_connectToOtherThreads);
	}
	else
	{
		return;
	}
}

void RebroadcastVoid::slot_connectToOtherThreads()
{
	Lock x(myLock);
	++numBroadcasts;
}
	
void RebroadcastVoid::slot_connectToSameThread()
{		
	signal_toOtherThreads.broadcast();
	slot_connectToOtherThreads();
}

void RebroadcastVoid::rebroadcast()
{
	Lock x(myLock);
	for(; numBroadcasts > 0; --numBroadcasts)
	{
		signal_toSameThread.broadcast();
	}
}


void RebroadcastDouble::requestReciprocalConnection(Rebroadcast_base* rhs)
{
	RebroadcastDouble* other = rhs->asDouble();
	if(other != NULL)
	{
		// Note: 'other' is the RebroadcastDouble to connect to (intended to be in another thread)
		signal_toOtherThreads.connect(other, &RebroadcastDouble::slot_connectToOtherThreads);
		other->signal_toOtherThreads.connect(this, &RebroadcastDouble::slot_connectToOtherThreads);
	}
	else
	{
		return;
	}
}
void RebroadcastDouble::setVariableToUpdate(double* var)
{
	var_ptr = var;
}
void RebroadcastDouble::slot_connectToOtherThreads(double value)
{
	Lock x(myLock);
	signal_list.push_back(value);
}
void RebroadcastDouble::slot_connectToSameThread(double value)
{		
	signal_toOtherThreads.broadcast(value);
	slot_connectToOtherThreads(value);
}
void RebroadcastDouble::rebroadcast()
{
	Lock x(myLock);
	std::list<double>::iterator iter = signal_list.begin();
	for(; iter!=signal_list.end();++iter)
	{
		signal_toSameThread.broadcast(*iter);
		if(var_ptr) *var_ptr = *iter;
	}
	signal_list.clear();
}

void RebroadcastString::requestReciprocalConnection(Rebroadcast_base* rhs)
{
	RebroadcastString* other = rhs->asString();
	if(other != NULL)
	{
		// Note: 'other' is the RebroadcastString to connect to (intended to be in another thread)
		signal_toOtherThreads.connect(other, &RebroadcastString::slot_connectToOtherThreads);
		other->signal_toOtherThreads.connect(this, &RebroadcastString::slot_connectToOtherThreads);
	}
	else
	{
		return;
	}
}

void RebroadcastString::slot_connectToOtherThreads(std::string str)
{
	Lock x(myLock);
	signal_list.push_back(str);
}

void RebroadcastString::slot_connectToSameThread(std::string str)
{
	signal_toOtherThreads.broadcast(str);
	slot_connectToOtherThreads(str);
}
void RebroadcastString::rebroadcast()
{
	Lock x(myLock);
	std::list<std::string>::iterator iter = signal_list.begin();
	for(; iter!=signal_list.end();++iter)
	{
		signal_toSameThread.broadcast(*iter);
	}
	signal_list.clear();
}
