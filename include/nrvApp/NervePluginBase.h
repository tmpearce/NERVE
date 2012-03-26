#pragma once
class QWidget;
class NerveAPI;

#define EXPECTED_VERSION 1

class NervePluginBase
{
public:
	virtual ~NervePluginBase(){}
	virtual void init(NerveAPI* api) = 0;
	virtual void calledInLoopDuringThreadedMode(){}
	virtual void callbackFromMainApplicationThread(int call_id){}
	virtual void acceptChildUI(QWidget* ui){}
};

//class QDialog;
//class NervePluginBase
//{
//public:
//	virtual bool handleUI(QDialog* ui){return false;}
//	virtual const char* writePluginState(){return 0;}
//	virtual void restorePluginState(const char* str){}
//	virtual void exec(){}//this function will be called in a loop if useThreadedMode() is called in the plugin api
//	virtual ~NervePluginBase(){}
//};