#pragma once
#include "nrv/MessageQueue.h"
class QThread;
class Pause;
class FunctionQueueQt
{
public:
	struct Functionoid
	{
		virtual void operator()() = 0;
		~Functionoid(){}
	};

	FunctionQueueQt();
	void callFunctionoidBlocking(Functionoid& f);
	void callFunctionoidNonblocking(Functionoid* f);
	void exec();
private:
	typedef std::pair<Functionoid*, Pause*> messageType;
	const QThread* _qAppThread;
	MessageQueue<messageType> messageQueue;
};