#include "nrvApp/NerveApplication.h"
#include "windows.h"
#include <iostream>
#include "nrvApp/GeneralGui.h"
#include <QtCore/QObject>
class QDialog;

void PluginCreator::operator ()()
{
	//h->setupAndRun();
}
NerveApplication::NerveApplication():qAppExec(0)
{
	OpenThreads::Thread::Init(); //this call is needed for threading to work properly
	int ac = 0;
	char ** av = NULL;
	app = new QApplication(ac, av);
	gui = new GeneralGui(this);

}
std::vector<std::string> NerveApplication::refreshPlugins()
{
	GeneralGui::PluginPathInfo p = gui->getPluginPathInfo();
	pluginRegistry.discoverPlugins(p.first,p.second);
	return getAvailableFactoryIDs();
}
bool NerveApplication::isQObjectInQApplicationThread(QObject* o)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(myMutex);
	if(app) return app->thread() == o->thread();
	else return false;
}
void NerveApplication::launch()
{
	timeBeginPeriod(2);

	// Not working yet
	/*HWND window = GetConsoleWindow();
	SMALL_RECT windowpos;
	windowpos.Top = 70;
	windowpos.Right = 700;
	windowpos.Bottom = 653;
	windowpos.Left = 100;
	SetConsoleWindowInfo(window, false, &windowpos);*/

	refreshPlugins();
	gui->refreshPluginList();
	gui->init();
	gui->move(300,50);
	gui->setVisible(true);

	qtimer.start();
	//control stops at app->exe() until QApplication returns
	app->exec();
	Sleep(10);
	pluginManager.clearAllPlugins();
	delete gui;
	delete app;
	std::cout<<"Exiting NERVEApplication\n";
	system("pause");
	timeEndPeriod(2);
}

