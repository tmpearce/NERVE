#include "osgwindow_interface.h"
#include "IImpl.h"

IOSGWindow::IOSGWindow(Window *w)
{
	pIImpl = new IImpl(w);
}
void IOSGWindow::setScene(osg::Group* n)
{
	pIImpl->setScene(n);
}
void IOSGWindow::setCameraCallback(osg::NodeCallback* n)
{
	pIImpl->setCameraCallback(n);
}
void IOSGWindow::setupInWindow(){pIImpl->setupInWindow();}
void IOSGWindow::setupFullscreen(){pIImpl->setupFullscreen();}
void IOSGWindow::setMonitorNum(int i){pIImpl->setMonitorNum(i);}
void IOSGWindow::setMirrorLR(bool b){pIImpl->setMirrorLR(b);}
void IOSGWindow::setMaxFrameRate(float fr){pIImpl->setMaxFrameRate(fr);}
void IOSGWindow::setUseStereoMode(bool b){pIImpl->setUseStereoMode(b);}
void IOSGWindow::setCameraMode(int i){pIImpl->setCameraMode(i);}
void IOSGWindow::applyChanges(){pIImpl->applyChanges();}