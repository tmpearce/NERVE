#include "nrv/PluginDeveloperTools.h"
#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginFactory.h"
#include "nrvApp/NervePluginBase.h"

M_MAKE_PLUGIN(TutorialPluginFactory)

class TutorialPlugin : public NervePluginBase
{
public:
	TutorialPlugin(NerveAPI* n){mpAPI = n;}
private:
	NerveAPI* mpAPI;
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