//include files for making a plugin
#include "nrv/PluginDeveloperTools.h"
#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginFactory.h"
#include "nrvApp/NervePluginBase.h"

//files from this project
#include "GuiHeader.h"
#include "OptotrakModule.h"
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
private:
	NerveAPI* mpAPI;
	OptotrakGui* gui;
	OptotrakModule optotrakModule;
	void createGui()
	{
		gui=new OptotrakGui();
		mpAPI->exposeUI(gui);
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
	return "Optotrak Interface";
}