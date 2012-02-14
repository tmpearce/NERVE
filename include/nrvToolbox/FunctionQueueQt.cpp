#include "nrvToolbox/FunctionQueueQt.h"
#include "nrvToolbox/Pause.h"
#include <QtGui/QApplication>
#include <QtCore/QThread>

FunctionQueueQt::FunctionQueueQt():_qAppThread(QApplication::instance() ? QApplication::instance()->thread() : 0){}

void FunctionQueueQt::callFunctionoidBlocking(Functionoid& f)
{
	//if the call comes from the QApplication thread, execute the functionoid immediately
	if(QThread::currentThread() == _qAppThread)
	{
		f();
	}
	else //call is from another thread, add it to the queue and wait until it is executed
	{
		Pause p;
		messageQueue.addMessage(messageType(&f,&p));
		p.pause();
	}
}
void FunctionQueueQt::callFunctionoidNonblocking(Functionoid* f)
{
	//if the call comes from the QApplication thread, execute the functionoid immediately
	if(QThread::currentThread() == _qAppThread)
	{
		(*f)();
		delete f;
	}
	else //call is from another thread, add it to the queue and wait until it is executed
	{
		messageQueue.addMessage(messageType(f,0));
	}
}
void FunctionQueueQt::exec()
{
	if(! messageQueue.messagesAreQueued()) return;
	std::list<messageType> mList = messageQueue.getMessages();
	for(std::list<messageType>::iterator iter = mList.begin();iter!=mList.end();++iter)
	{
		iter->first->operator()();
		Pause* p = iter->second;
		if(p) p->unpause();
		else delete iter->first;
	}
}