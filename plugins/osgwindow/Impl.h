#pragma once
#include "osgwindow_plugin.h"
#include "nrvApp/NerveAPI.h"
#include "osgwindow_guts.h"
#include "GuiHeader.h"

class Impl
{
public:
	enum CallbackID
	{
		CREATE_GUI,
		DESTROY_GUI
	};
	Impl():pluginObj(0),mpAPI(0),gui(0){}
	void init(osgwindow* plugin, NerveAPI* n)
	{
		pluginObj = plugin;
		mpAPI = n;
		mpAPI->callPluginFromMainThread(pluginObj,CREATE_GUI, NerveAPI::CALLBACK_REQUESTS_BLOCKING);
	}
	~Impl()
	{
		mpAPI->callPluginFromMainThread(pluginObj,DESTROY_GUI,NerveAPI::CALLBACK_REQUESTS_BLOCKING);
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
	osgwindow* pluginObj;
	osgwindowGui* gui;
	Window window;

	void createGui()
	{
		gui=new osgwindowGui(&window);
		mpAPI->exposeUI(gui);printf("Impl createGui 3\n");
	}	
	void destroyGui()
	{
		mpAPI->removeUI(gui);
		delete gui;
	}
	Impl(Impl& other);
};