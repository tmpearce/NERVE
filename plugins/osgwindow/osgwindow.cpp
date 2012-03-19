//include files for making a plugin
#include "nrv/PluginDeveloperTools.h"
//#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginFactory.h"
#include "nrvApp/NervePluginBase.h"

//files from this project
#include "osgwindow_plugin.h"
M_MAKE_PLUGIN(TutorialPluginFactory)


void TutorialPluginFactory::cleanUpPluginObject(NervePluginBase * p, NerveAPI * n)
{
	delete p;
}
NervePluginBase* TutorialPluginFactory::createPluginObject(NerveAPI * n)
{
	return new osgwindow(n);
}
const char* TutorialPluginFactory::getName()
{
	return "OSG Window";
}