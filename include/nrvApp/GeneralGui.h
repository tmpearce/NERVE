//  class GeneralGui: Qt-based main window - the base for the graphic
//	user interface (GUI) for the application.  GeneralGui lets
//	users select different interfaces for selecting and running tasks.
//	It also expands to include the settings gui for the currently
//	running task (if one has been implemented).


#pragma once


#include "ui_GeneralGui.h"
#include "nrvApp/SettingsEditor.h"
#include <QtGui/QActionGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QTabWidget>
#include <QtGui/QDockWidget>
#include <QtCore/QSet>
#include <QtGui/QAction>
#include <QtCore/QSignalMapper>
#include <QtGui/QContextMenuEvent>
#include <QtCore/QThread>
#include <QtGui/QDialog>
#include "nrv/EventObserver.h"
#include "nrvApp/NerveApplication.h"
#include "nrvToolbox/Pause.h"
#include "nrv/MessageQueue.h"



//class CentralWidget;
class UIDock;
class SettingsEditor;
class QAppExecutor : public QApplicationExecutor
{
	typedef std::pair<QApplicationExecutor::Functionoid*, Pause*> messageType;
public:
	QAppExecutor(QThread* appEventLoopThreadPtr):_appThread(appEventLoopThreadPtr){}
	void callFunctionoidBlocking(QApplicationExecutor::Functionoid& f)
	{
		//if the call comes from the QApplication thread, execute the functionoid immediately
		if(QThread::currentThread() == _appThread)
		{
			f();
		}
		else //call is from another thread, add it to the queue and wait until it is executed
		{
			Pause p;
			messageQueue.addMessage(messageType(&f,&p));
			p.pause();
		}
	}
	void callFunctionoidNonblocking(QApplicationExecutor::Functionoid& f)
	{
		//if the call comes from the QApplication thread, execute the functionoid immediately
		if(QThread::currentThread() == _appThread)
		{
			f();
		}
		else //call is from another thread, add it to the queue and return immediately
		{
			Pause p;
			messageQueue.addMessage(messageType(&f,&p));
			//p.pause(); Don't actually pause - that would make it blocking
		}
	}
private:
	const QThread* _appThread;
	MessageQueue<messageType> messageQueue;
	void exec()
	{
		std::list<messageType> mList = messageQueue.getMessages();
		for(std::list<messageType>::iterator iter = mList.begin();iter!=mList.end();++iter)
		{
			iter->first->operator()();
			iter->second->unpause();
		}
	}
	friend class GeneralGui;
};

class GeneralGui : public QMainWindow
{
	Q_OBJECT
public:
	typedef std::pair<std::string,bool> PluginPathInfo;
private:
signals:
	void pluginAvailableSignal(const QString&);
	void pluginRemovedSignal(const QString&);
	void uiAvailableSignal(const QString&,const QString&,QWidget*);
	void uiRemovedSignal(const QString&,const QString&,QWidget*);
	void uiAvailableSignalBlocking(const QString&,const QString&,QWidget*);
	void uiRemovedSignalBlocking(const QString&,const QString&,QWidget*);
public:
	GeneralGui(NerveApplication* napp, QWidget * parent = 0);	
	PluginPathInfo getPluginPathInfo();
	void pluginAvailable(std::string id){ emit pluginAvailableSignal(id.c_str()); }
	void pluginUnavailable(std::string id){	emit pluginRemovedSignal(id.c_str()); }
	void uiAvailable(UIInfo info);
	void uiRemoved(UIInfo info);
	void init();
private slots:
	void addPlugin(QString id, bool b=false);
	void removePlugin(QString id);
	void addPluginUI(QString id, QString title, QWidget* ui);
	void removePluginUI(QString id, QString title, QWidget* ui);
	void toggleSettings();
public slots:
	void createPlugin(QString id);
	void quitPlugin(QString id);
	void refreshPluginList();
	void update();
	void pluginDockChanged(QString id, bool floating);
	void loadConfig(QString file);
	void updateRecentFiles(QString file);
	void saveConfigAs();
	QMenu* createPopupMenu(){return new QMenu(this);}
private:
	Ui::GeneralGui ui;
	
protected:
	void closeEvent(QCloseEvent* = 0);
	void customResize();

	NerveApplication* nerveApp;
	QAppExecutor appExecutor;
	CallbackObserver<std::string, GeneralGui> PluginCreated;
	CallbackObserver<std::string, GeneralGui> PluginClosed;
	CallbackObserver<UIInfo, GeneralGui>	  UIAvailable;
	CallbackObserver<UIInfo, GeneralGui>	  UIRemoved;

	const QThread* qAppThread;
	QSettings* appSettings;
	std::map<QString, UIDock*> pluginDockMap;
	QSet<QString> dockedPlugins;
	QSignalMapper pluginCreationMapper;
	QSignalMapper loadRecentMapper;
	QStringList loadRecentFiles;
	SettingsEditor* settingsEditor;

public slots:
	void quit();
};


class UIDock : public QDockWidget
{
	Q_OBJECT
public:
	UIDock(QString id, bool onlyUI):myID(id),floatingStatus(false),isOnlyUI(onlyUI)
	{
		tabWidget = new QTabWidget(this);
		myAction = new QAction(id,this);
		myAction->setMenu(new QMenu(this));

		QAction* showAction = new QAction(QString("Shown"),this);
		showAction->setCheckable(true);
		showAction->setChecked(true);
		connect(showAction,SIGNAL(triggered(bool)),this,SLOT(showDock(bool)));
		connect(this, SIGNAL(dockShowStatusChanged(bool)),showAction,SLOT(setChecked(bool)));		

		QAction* floatAction = new QAction(QString("Floating"),this);
		floatAction->setCheckable(true);
		floatAction->setChecked(false);
		connect(floatAction,SIGNAL(triggered(bool)),this,SLOT(floatDock(bool)));
		connect(this, SIGNAL(topLevelChanged(bool)),floatAction,SLOT(setChecked(bool)));		

		QAction* quitAction = new QAction(QString("Quit"),this);
		connect(quitAction,SIGNAL(triggered()),this,SLOT(requestQuit()));
		connect(this,SIGNAL(uiOnly(bool)),quitAction,SLOT(setEnabled(bool)));
		quitAction->setEnabled(!isOnlyUI);//disable the quit function if the application isn't the owner

		myAction->menu()->addAction(showAction);
		myAction->menu()->addAction(floatAction);
		myAction->menu()->addSeparator();
		myAction->menu()->addAction(quitAction);

		myContextMenu = new QMenu(this);
		myContextMenu->addAction(showAction);
		myContextMenu->addAction(floatAction);

		setWidget(tabWidget);
		tabWidget->setMovable(true);
		tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		//tabWidget->setMinimumSize(1,1);
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(dockFloatingChanged(bool)));
	}
	void addUITab(QWidget* ui, QString label)
	{
		setUpdatesEnabled(false);
		tabWidget->addTab(ui, label);
		setUpdatesEnabled(true);
		tabWidget->show();
	}
	void removeUITab(QWidget* ui)
	{
		tabWidget->removeTab(tabWidget->indexOf(ui));
		if(tabWidget->count() == 0) tabWidget->hide();
	}
	void removeAllUITabs()
	{
		while(tabWidget->count()>0)
		{
			QWidget* widget = tabWidget->widget(0);
			tabWidget->removeTab(0);
			widget->setParent(0);
		}
	}
	QAction* getAction(){return myAction;}
	void closeEvent(QCloseEvent* evnt)
	{
		emit dockShowStatusChanged(false);
		QDockWidget::closeEvent(evnt);
	}
	void contextMenuEvent(QContextMenuEvent *evnt)
	{
		 myContextMenu->popup(evnt->globalPos());
	}
	bool getIsOnlyUI(){return isOnlyUI;}
	void setIsOnlyUI(bool b)
	{
		if(b==isOnlyUI) return;
		isOnlyUI=b;
		emit uiOnly(!isOnlyUI);
		/*QList<QAction*> actionList = myAction->menu()->actions();
		for(int i=0;i<actionList.size();++i)
		{
			if(actionList.at(i)->text()=="Quit")
				actionList.at(i)->setEnabled(!isOnlyUI);
		}*/
	}
	int getNumUIs(){return tabWidget->count();}
public slots:
	void dockFloatingChanged(bool floating){floatingStatus = floating;emit dockStatusChanged(myID,floating);}
	void showDock(bool b){setVisible(b);}
	void floatDock(bool b){setFloating(b);}
	void requestQuit(){emit quit(myID);}
signals:
	void dockStatusChanged(QString id, bool floating);
	void dockShowStatusChanged(bool b);
	void quit(QString id);
	void uiOnly(bool b);
protected:
	QTabWidget* tabWidget;
	QAction* myAction;
	QMenu* myContextMenu;
	QString myID;
	bool floatingStatus;
	bool isOnlyUI;
};