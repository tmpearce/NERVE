#pragma once
#include "nrvApp/NervePluginBase.h"
#include "nrvApp/NerveAPI.h"
#include "GuiHeader.h"
#include "osgwindow_guts.h"

class osgwindow : public NervePluginBase
{
public:
	enum CallbackID
	{
		CREATE_GUI,
		DESTROY_GUI
	};
	osgwindow(NerveAPI* n)
	{
		mpAPI = n;
		mpAPI->callPluginFromMainThread(this,CREATE_GUI, NerveAPI::CALLBACK_REQUESTS_BLOCKING);
	}
	~osgwindow()
	{
		mpAPI->callPluginFromMainThread(this,DESTROY_GUI,NerveAPI::CALLBACK_REQUESTS_BLOCKING);
	}
	void callbackFromMainApplicationThread(int call_id)
	{
		switch(call_id)
		{
		case CREATE_GUI: createGui(); break;
		case DESTROY_GUI: destroyGui(); break;
		}
	}
private:
	NerveAPI* mpAPI;
	osgwindowGui* gui;
	Window window;

	void createGui()
	{
		gui=new osgwindowGui(&window);
		mpAPI->exposeUI(gui);
	}
	void destroyGui()
	{
		mpAPI->removeUI(gui);
		delete gui;
	}
	osgwindow(osgwindow& other);
};
