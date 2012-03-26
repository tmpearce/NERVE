//include files for making a plugin
#include "nrv/PluginDeveloperTools.h"
#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginFactory.h"
#include "nrvApp/NervePluginBase.h"
#include "osgwindow_interface.h"

//files from this project
#include "GuiHeader.h"
#include "SceneElements.h"
M_MAKE_PLUGIN(TutorialPluginFactory)

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
		printf("Creating mobile element\n");
		MobileElement* me = new MobileElement();
		printf("Destroying mobile element\n");
		delete me;
	}
	~TutorialPlugin()
	{
		if(iWindow)
		{
			mpAPI->unbindIPlugin(iWindow);
		}
		mpAPI->cancelChildPlugin(window_plugin);
		mpAPI->callPluginFromMainThread(this,DESTROY_GUI,NerveAPI::CALLBACK_REQUESTS_BLOCKING);
		printf("after blocking callback in window user dtor\n");
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
private:
	NerveAPI* mpAPI;
	TutorialGui* gui;
	IOSGWindow* iWindow;
	std::string window_plugin;

	void createGui()
	{
		gui=new TutorialGui();
		mpAPI->exposeUI(gui);
		mpAPI->setTakeOwnershipOfCreatedPlugins(true);
		mpAPI->setWillAcceptChildUIs(true);
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
	}
	void destroyGui()
	{
		mpAPI->removeUI(gui);
		delete gui;
	}
};

void TutorialPluginFactory::cleanUpPluginObject(NervePluginBase * p, NerveAPI * n)
{
	delete p;
}
NervePluginBase* TutorialPluginFactory::createPluginObject()
{
	return new TutorialPlugin();
}


const char* TutorialPluginFactory::getName()
{
	return "Tutorial Plugin";
}