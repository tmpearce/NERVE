#include "osgwindow_plugin.h"
#include "GuiHeader.h"
#include "osgwindow_guts.h"
#include "Impl.h"

osgwindow::osgwindow()
{
	pImpl = new Impl();
	
}
void osgwindow::init(NerveAPI* n)
{
	pImpl->init(this,n);
}
osgwindow::~osgwindow(){delete pImpl;}
void osgwindow::callbackFromMainApplicationThread(int call_id)
{
	pImpl->callbackFromMainApplicationThread(call_id);
}