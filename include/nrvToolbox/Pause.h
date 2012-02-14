#pragma once
#include <OpenThreads/Atomic>
#include <OpenThreads/Thread>
class Pause
{
public:
	Pause():atomic(1){}
	void pause(){while(atomic==1){OpenThreads::Thread::microSleep(10);}}
	void unpause(){--atomic;}
	bool isPaused(){return atomic==1;}
protected:
	OpenThreads::Atomic atomic;
};