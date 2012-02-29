#pragma once
#include "nrvApp/NervePluginBase.h"
#include "nrvApp/NerveAPI.h"
#include "Gui.h"

class TutorialPlugin : public NervePluginBase
{
public:
	enum CallbackID
	{
		CREATE_GUI,
		DESTROY_GUI
	};
	TutorialPlugin(NerveAPI* n)
	{
		mpAPI = n;
		mpAPI->callPluginFromMainThread(this,CREATE_GUI, NerveAPI::CALLBACK_REQUESTS_BLOCKING);
	}
	~TutorialPlugin()
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
	void create(std::string pname)
	{
		mpAPI->createPlugin(pname);
	}
	void refreshAvailable()
	{
		NerveAPI::StringList l = mpAPI->getAvailableFactoryIDs();
		gui->setAvailablePlugins(l);
	}
	void toggleTakeOwnership(bool b){mpAPI->setTakeOwnershipOfCreatedPlugins(b);}
	void toggleHandleChildUIs(bool b){mpAPI->setWillAcceptChildUIs(b);}
private:
	NerveAPI* mpAPI;
	TutorialGui* gui;

	void createGui()
	{
		gui=new TutorialGui(this);
		mpAPI->exposeUI(gui);
		refreshAvailable();		
	}
	void destroyGui()
	{
		mpAPI->removeUI(gui);
		delete gui;
		
	}
};