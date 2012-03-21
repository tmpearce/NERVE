//include files for making a plugin
#include "nrv/PluginDeveloperTools.h"
#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginFactory.h"
#include "nrvApp/NervePluginBase.h"
#include "osgwindow_plugin.h"

//files from this project
#include "GuiHeader.h"
M_MAKE_PLUGIN(TutorialPluginFactory)

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
	void acceptChildUI(QWidget* ui)
	{
		gui->acceptChildUI(ui);
	}
private:
	NerveAPI* mpAPI;
	TutorialGui* gui;

	void createGui()
	{
		gui=new TutorialGui();
		mpAPI->exposeUI(gui);
		mpAPI->setTakeOwnershipOfCreatedPlugins(true);
		mpAPI->setWillAcceptChildUIs(true);
		std::string window_plugin = mpAPI->createPlugin("OSG Window <osgwindow.dll>");
		//mpAPI->get
		//printf("trying to create osgwindow: %s\n",mpAPI->createPlugin("OSG Window <osgwindow.dll>").c_str());
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
NervePluginBase* TutorialPluginFactory::createPluginObject(NerveAPI * n)
{
	return new TutorialPlugin(n);
}


const char* TutorialPluginFactory::getName()
{
	return "Tutorial Plugin";
}