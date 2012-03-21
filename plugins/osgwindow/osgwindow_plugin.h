#pragma once
#include "nrvApp/NervePluginBase.h"
class NerveAPI;
class Impl;
class osgwindow : public NervePluginBase
{
public:
	
	osgwindow();
	~osgwindow();
	void init(NerveAPI* api);
	void callbackFromMainApplicationThread(int call_id);
private:
	Impl* pImpl;
	osgwindow(osgwindow& other);
};
