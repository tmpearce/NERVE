#pragma once
#include "osgwindow_plugin.h"
#include "nrvApp/NerveAPI.h"
#include "osgwindow_guts.h"
#include "GuiHeader.h"
#include "osgwindow_interface.h"

class Impl
{
public:
	enum CallbackID
	{
		CREATE_GUI,
		DESTROY_GUI
	};
	Impl():pluginObj(0),mpAPI(0),gui(0),iWindow(&window){}
	void init(osgwindow* plugin, NerveAPI* n)
	{
		pluginObj = plugin;
		mpAPI = n;
		mpAPI->callPluginFromMainThread(pluginObj,CREATE_GUI, NerveAPI::CALLBACK_REQUESTS_BLOCKING);
		mpAPI->exposeIPlugin(&iWindow);
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
	IOSGWindow iWindow;

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
	Impl(Impl& other);
};