//include files for making a plugin
#include "nrv/PluginDeveloperTools.h"
#include "nrvApp/NervePluginFactory.h"

#include "plugin.h"
M_MAKE_PLUGIN(CenterOutFactory)


void CenterOutFactory::cleanUpPluginObject(NervePluginBase * p, NerveAPI * n)
{
	delete p;
}
NervePluginBase* CenterOutFactory::createPluginObject()
{
	return new CenterOutPlugin();
}


const char* CenterOutFactory::getName()
{
	return "Center Out";
}