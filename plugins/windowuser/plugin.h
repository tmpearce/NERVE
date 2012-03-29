#pragma once
#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginBase.h"
#include "osgwindow_interface.h"
#include "nrvThread/NerveThread.h"
#include "nrvThread/NerveModule.h"

//files from this project
#include "plugin.h"
#include "GuiHeader.h"
#include "CenterOut.h"


class PositionModule : public NerveModule
{
public:
	PositionModule(CenterOut* cenout, IOSGWindow* window, float scale):co(cenout),w(window),s(scale){}
	void moduleOperation(NerveModuleUser*)
	{
		CenterOut::CursorPosition pos;
		IOSGWindow::Position mpos = w->getMousePosition();
		pos.x = mpos.first * s;
		pos.y = 0.f;
		pos.z = mpos.second * s;
		co->setCursorPosition(pos);
		OpenThreads::Thread::microSleep(1000);
	}
private:
	CenterOut* co;
	IOSGWindow* w;
	float s;
};

class TutorialPlugin : public NervePluginBase
{
public:
	enum CallbackID
	{
		CREATE_GUI,
		DESTROY_GUI
	};
	void init(NerveAPI* n)
	{
		mpAPI = n;
		mpAPI->callPluginFromMainThread(this,CREATE_GUI, NerveAPI::CALLBACK_REQUESTS_BLOCKING);
		posThread.start();
	}
	~TutorialPlugin()
	{
		posThread.cancel();
		if(iWindow)
		{
			mpAPI->unbindIPlugin(iWindow);
		}
		mpAPI->cancelChildPlugin(window_plugin);
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
	void acceptChildUI(QWidget* ui)
	{
		gui->acceptChildUI(ui);
	}

	void createWindow()
	{
		window_plugin = mpAPI->createPlugin("OSG Window <osgwindow.dll>");
		IPlugin* ip = mpAPI->bindIPlugin(window_plugin);
		iWindow = dynamic_cast<IOSGWindow*>(ip);
		if(iWindow)
		{
			printf("successfully cast IPlugin* to IOSGWindow*\n");
		}
		else
		{
			printf("failed to cast IPlugin* to IOSGWindow*\n");
		}
		iWindow->setScene(cenOut.getScene());
		iWindow->setCameraCallback(cenOut.getCameraCallback());
		iWindow->initWindowEvents();

		PositionModule* m = new PositionModule(&cenOut, iWindow, 20.f);
		m->setOperateAction(NerveModule::DONT_REMOVE_MODULE);
		m->setRemoveAction(NerveModule::DELETE_MODULE);
		posThread.addModule(*m);
	}
private:
	NerveThread posThread;
	NerveAPI* mpAPI;
	TutorialGui* gui;
	IOSGWindow* iWindow;
	std::string window_plugin;
	CenterOut cenOut;

	void createGui()
	{
		gui=new TutorialGui(this);
		mpAPI->exposeUI(gui);
		mpAPI->setTakeOwnershipOfCreatedPlugins(true);
		mpAPI->setWillAcceptChildUIs(true);
		
	}
	void destroyGui()
	{
		mpAPI->removeUI(gui);
		delete gui;
	}
};
