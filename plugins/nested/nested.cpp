//include files for making a plugin
#include "nrv/PluginDeveloperTools.h"
#include "nrvApp/NervePluginFactory.h"
#include "nrvApp/NervePluginBase.h"

//files from this project
#include "TutorialPlugin.h"

M_MAKE_PLUGIN(TutorialPluginFactory)



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
	return "Tutorial - Nested Plugins";
}
