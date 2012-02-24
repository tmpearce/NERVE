//include files for making a plugin
#include "nrv/PluginDeveloperTools.h"
#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginFactory.h"
#include "nrvApp/NervePluginBase.h"

//files from this project
#include "GuiHeader.h"
M_MAKE_PLUGIN(TutorialPluginFactory)

class TutorialPlugin : public NervePluginBase
{
public:
	enum CallbackID
	{
		CREATE_GUI
	};
	TutorialPlugin(NerveAPI* n)
	{
		mpAPI = n;
		printf("pointer copied\n");
		mpAPI->requestCallbackFromMainApplicationThread(CREATE_GUI, NerveAPI::CALLBACK_REQUESTS_BLOCKING);
	}
	void callbackFromMainApplicationThread(int call_id)
	{
		printf("callback\n");
		switch(call_id)
		{
		case CREATE_GUI: createGui(); break;
		}
	}
private:
	NerveAPI* mpAPI;
	TutorialGui* gui;

	void createGui()
	{
		gui=new TutorialGui();
		mpAPI->exposeUI(gui);
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