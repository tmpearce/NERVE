#include "nrvApp\GeneralGui.h"
#include "nrvApp\NerveApplication.h"
#include <QtCore\QTimer>
#include <QtGui\QDockWidget>
#include <QtGui\QAction>
#include <QtCore\QThread>



GeneralGui::GeneralGui(NerveApplication* app, QWidget * parent):
PluginCreated(this, &GeneralGui::pluginAvailable),
PluginClosed(this, &GeneralGui::pluginUnavailable),
UIAvailable(this, &GeneralGui::uiAvailable),
UIRemoved(this, &GeneralGui::uiRemoved),
appExecutor(QApplication::instance()->thread()),
qAppThread(QApplication::instance()->thread())
{
	nerveApp = app;
	ui.setupUi(this);
	QTimer* timer = new QTimer(this);
	timer->start(100);
	
	appSettings = new QSettings("Moran Lab","NERVE");
	settingsEditor = new SettingsEditor(appSettings, this);
	settingsEditor->hide();

	connect(timer,SIGNAL(timeout()),this,SLOT(update()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(quit()));
	connect(&pluginCreationMapper,SIGNAL(mapped(QString)),this,SLOT(createPlugin(QString)));
	connect(this, SIGNAL(pluginAvailableSignal(QString)),this, SLOT(addPlugin(QString)));
	connect(this, SIGNAL(pluginRemovedSignal(QString)),this, SLOT(removePlugin(QString)));
	connect(this, SIGNAL(uiAvailableSignal(QString,QString,QWidget*)),this, SLOT(addPluginUI(QString,QString,QWidget*)),Qt::DirectConnection);
	connect(this, SIGNAL(uiRemovedSignal(QString,QString,QWidget*)),this, SLOT(removePluginUI(QString,QString,QWidget*)),Qt::DirectConnection);
	connect(this, SIGNAL(uiAvailableSignalBlocking(QString,QString,QWidget*)),this, SLOT(addPluginUI(QString,QString,QWidget*)),Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(uiRemovedSignalBlocking(QString,QString,QWidget*)),this, SLOT(removePluginUI(QString,QString,QWidget*)),Qt::BlockingQueuedConnection);
	connect(ui.actionEdit,SIGNAL(triggered()),this,SLOT(toggleSettings()));
	connect(ui.actionRefresh_Plugins,SIGNAL(triggered()),this,SLOT(refreshPluginList()));
	connect(ui.actionSave_as,SIGNAL(triggered()),this,SLOT(saveConfigAs()));
	
	ui.centralwidget->hide();
	setDockOptions(QMainWindow::VerticalTabs | QMainWindow::ForceTabbedDocks);
	
	nerveApp->setQApplicationExecutor(&appExecutor);//allows the application to execute calls from the qt event loop (to spawn guis)
	nerveApp->addEventObserver("PluginAvailable", PluginCreated);
	nerveApp->addEventObserver("PluginRemoved", PluginClosed);
	nerveApp->addEventObserver("UIAvailable", UIAvailable);
	nerveApp->addEventObserver("UIRemoved", UIRemoved);

	
	QString s;
	s = appSettings->value("main/recent/1").toString();
	if(!s.isEmpty())
	{
		QAction* action = new QAction(s,this);
		loadRecentMapper.setMapping(action,s);
		connect(action,SIGNAL(triggered()),&loadRecentMapper,SLOT(map()));
		//ui.menuFile->addAction(
	}
	connect(&loadRecentMapper,SIGNAL(mapped(QString)),this,SLOT(loadConfig(QString)));
}
void GeneralGui::init()//called after plugins are detected
{
	settingsEditor->loadSettings();
	QString p = settingsEditor->getStartupPlugin();
	if(!p.isEmpty())
	{
		createPlugin(p);
	}
}
/*******Slot definitions for GUI actions***********/
void GeneralGui::quit()
{
	closeEvent();
	close();
}
void GeneralGui::closeEvent(QCloseEvent* qcloseevent)
{	
	nerveApp->setQApplicationExecutor(0);
	update();
}
void GeneralGui::pluginDockChanged(QString id, bool floating)
{
	if(floating) dockedPlugins.remove(id);
	else 
	{
		if(dockedPlugins.size() > 0)tabifyDockWidget(pluginDockMap[*dockedPlugins.begin()],pluginDockMap[id]);
		dockedPlugins.insert(id);
	}
	
}

void GeneralGui::customResize()
{
	printf("GeneralGui pre size %i %i\n",size().width(),size().height());
	adjustSize();
	printf("GeneralGui post size %i %i\n",size().width(),size().height());
}
void GeneralGui::createPlugin(QString id){nerveApp->createPlugin(id.toStdString(),NerveApplication::PLUGIN_OWNED_BY_APPLICATION);}
void GeneralGui::addPlugin(QString id)
{
	if(pluginDockMap.count(id) == 0)//create a UIDock for this plugin
	{
		UIDock* dock = new UIDock(id);		
		dock->setWindowTitle(id);
		dock->setAllowedAreas(Qt::RightDockWidgetArea);
		dock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

		pluginDockMap[id] = dock;
		if(dockedPlugins.size() > 0)
		{
			tabifyDockWidget(pluginDockMap[*dockedPlugins.begin()],dock);
		}
		else addDockWidget(Qt::RightDockWidgetArea,dock);
		dockedPlugins.insert(id);
		
		ui.menuPlugins->addAction(dock->getAction());
		connect(dock,SIGNAL(dockStatusChanged(QString,bool)),this,SLOT(pluginDockChanged(QString,bool)));
		connect(dock, SIGNAL(quit(QString)),this,SLOT(quitPlugin(QString)));
	}
}
void GeneralGui::removePlugin(QString id)
{
	if(pluginDockMap.count(id) > 0)
	{
		UIDock* dock = pluginDockMap[id];
		dock->removeAllUITabs();
		dock->deleteLater();
		pluginDockMap.erase(id);
		dockedPlugins.remove(id);
	}
}
void GeneralGui::refreshPluginList()
{
	std::vector<std::string> pluginTypes = nerveApp->refreshPlugins();
	QMenu* pluginMenu = new QMenu(this);
	ui.actionCreate_Plugin->setMenu(pluginMenu);
	QStringList qstringlist;
	for(unsigned int i=0;i<pluginTypes.size();++i)
	{
		QString pluginType(pluginTypes[i].c_str());
		qstringlist.push_back(pluginType);
		QAction* pluginAction = new QAction(pluginType, this);
		pluginCreationMapper.setMapping(pluginAction,pluginType);
		connect(pluginAction,SIGNAL(triggered()),&pluginCreationMapper,SLOT(map()));
		pluginMenu->addAction(pluginAction);
	}
	settingsEditor->setPluginTypes(qstringlist);
	
	//settingsEditor->refreshSettings();
}
void GeneralGui::toggleSettings()
{
	printf("toggleSettings\n");
	if(settingsEditor->isVisible()) settingsEditor->hide();
	else                            settingsEditor->show();
	/*if(ui.centralwidget->isVisible()) ui.centralwidget->hide();
	else ui.centralwidget->show();*/
}
void GeneralGui::loadConfig(QString file)
{
	settingsEditor->loadSettings();
}
void GeneralGui::updateRecentFiles(QString file)
{
}
void GeneralGui::saveConfigAs()
{
	settingsEditor->saveSettingsAs();
}
void GeneralGui::addPluginUI(QString id, QString title, QWidget* ui)
{
	if(pluginDockMap.count(id) == 0) addPlugin(id);
	
	if(ui)
	{
		ui->setMinimumSize(ui->width(),ui->height());
		ui->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
		pluginDockMap[id]->addUITab(ui, title);
	}
	
}
void GeneralGui::removePluginUI(QString id, QString title, QWidget* ui)
{
	if(pluginDockMap.count(id) > 0)
	{
		pluginDockMap[id]->removeUITab(ui);
		ui->setParent(0);//let nerve take care of deleting the ui instead qt
	}
}
void GeneralGui::quitPlugin(QString id)
{
	nerveApp->destroyPlugin(id.toAscii().constData());
}
void GeneralGui::update(){appExecutor.exec();}
void GeneralGui::uiAvailable(UIInfo info)
{
	if(qAppThread == QThread::currentThread())//emit signal hooked up with Qt::DirectConnection - executes and returns immediately
	{//printf("in qAppThread\n");
		emit uiAvailableSignal(info.plugin_id.c_str(),info.ui_title.c_str(),info.ui_ptr);
	}
	else //emit signal hooked up with Qt::BlockingQueuedConnection - blocks until executed in QApplication event loop
	{//printf("not in qAppThread\n");
		emit uiAvailableSignalBlocking(info.plugin_id.c_str(),info.ui_title.c_str(),info.ui_ptr);
	}
}
void GeneralGui::uiRemoved(UIInfo info)
{
	if(qAppThread == QThread::currentThread())//emit signal hooked up with Qt::DirectConnection - executes and returns immediately
	{//printf("in qAppThread\n");
		emit uiRemovedSignal(info.plugin_id.c_str(),info.ui_title.c_str(),info.ui_ptr);
	}
	else //emit signal hooked up with Qt::BlockingQueuedConnection - blocks until executed in QApplication event loop
	{//printf("not in qAppThread\n");
		emit uiRemovedSignalBlocking(info.plugin_id.c_str(),info.ui_title.c_str(),info.ui_ptr);
	}
}