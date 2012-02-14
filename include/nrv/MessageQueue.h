#pragma once
#include <OpenThreads\Atomic>
#include <OpenThreads\Mutex>
#include <OpenThreads\ScopedLock>
#include <list>

template <class T>
class MessageQueue
{
	typedef std::list<T> MQ;
public:
	MessageQueue()
	{
		readQueue = &queue1;
		writeQueue = &queue2;
	}
	void addMessage(T message)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
		++messagesQueued;//increment to non-zero indicating that messages are queued
		writeQueue->push_back(message);
	}
	bool messagesAreQueued(){return messagesQueued != 0;}
	std::list<T> getMessages()
	{
		
		{//only lock long enough to swap pointers to the buffers
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
			messagesQueued.exchange(0);
			MQ* temp(readQueue);
			readQueue = writeQueue;
			writeQueue = temp;
		}
		MQ queuedMessages(*readQueue);
		readQueue->clear();
		return queuedMessages;
	}
protected:
	OpenThreads::Atomic messagesQueued;
	OpenThreads::Mutex  myMutex;
	MQ queue1;
	MQ queue2;
	MQ* writeQueue;
	MQ* readQueue;
};