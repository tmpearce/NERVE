//  class GeneralGui: Qt-based main window - the base for the graphic
//	user interface (GUI) for the application.  GeneralGui lets
//	users select different interfaces for selecting and running tasks.
//	It also expands to include the settings gui for the currently
//	running task (if one has been implemented).


#pragma once


#include "nrvApp\_UIC_GeneralGui.h"
#include "nrvApp/_UIC_SettingsEditor.h"
#include <QtGui\QActionGroup>
#include <QtGui\QMainWindow>
#include <QtGui\QTabWidget>
#include <QtGui\QDockWidget>
#include <QtCore\QSet>
#include <QtGui\QAction>
#include <QtCore\QSignalMapper>
#include <QtGui\QContextMenuEvent>
#include <QtCore/QThread>
#include <QtGui\QDialog>
#include "nrv\EventObserver.h"
#include "nrvApp\NerveApplication.h"
#include "nrvToolbox/Pause.h"
#include "nrv/MessageQueue.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QVBoxLayout>
#include <QtGui/QCheckBox>
#include <QtCore/QSettings>

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
	void pluginAvailable(std::string id){ emit pluginAvailableSignal(id.c_str()); }
	void pluginUnavailable(std::string id){	emit pluginRemovedSignal(id.c_str()); }
	void uiAvailable(UIInfo info);
	void uiRemoved(UIInfo info);
	void init();
private slots:
	void addPlugin(QString id);
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

class SettingsEditor : public QDialog
{
	Q_OBJECT
public:
	class Checkbox : public QWidget
	{
	public:
		Checkbox(const QString& text_, QWidget* parent=0):text(text_),QWidget(parent)
		{
			QHBoxLayout* layout = new QHBoxLayout(this);
			qcheckbox = new QCheckBox(this);
			QLabel* label = new QLabel(text,this);
			label->setWordWrap(true);			
			layout->addWidget(qcheckbox);
			layout->addWidget(label);
		}
		~Checkbox(){}
		bool isChecked(){return qcheckbox->isChecked();}
		QString text;
		QCheckBox* qcheckbox;
	};
	class DynamicPluginLayout : public QVBoxLayout
	{
	public:
		DynamicPluginLayout(QWidget* parent = 0):QVBoxLayout(0){}
		void clear()
		{
			while(count()>0)
			{
				QLayoutItem *item = takeAt(0);
				
				delete item->widget();
				delete item;
			}
		}
		void setup(QVector<SettingsEditor::Checkbox*> c)
		{
			clear();
			for(int i=0; i<c.size();++i)
			{
				addWidget(c.at(i),0,Qt::AlignLeft);
			}
			addStretch(0);
		}
	};
	SettingsEditor(QWidget* parent)
	{
		ui.setupUi(this);

		connect(&signalMapper,SIGNAL(mapped(int)),ui.stackedWidget,SLOT(setCurrentIndex(int)));
		signalMapper.setMapping(ui.buttonStartup,1);
		connect(ui.buttonStartup,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonFavorites,2);
		connect(ui.buttonFavorites,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonMain_GUI,3);
		connect(ui.buttonMain_GUI,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonReturn,0);
		connect(ui.buttonReturn,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonReturn_2,0);
		connect(ui.buttonReturn_2,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonReturn_3,0);
		connect(ui.buttonReturn_3,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		
		applyMapper.setMapping(ui.buttonApply,1);
		connect(ui.buttonApply,SIGNAL(clicked()),&applyMapper,SLOT(map()));
		applyMapper.setMapping(ui.buttonApply_2,2);
		connect(ui.buttonApply_2,SIGNAL(clicked()),&applyMapper,SLOT(map()));
		applyMapper.setMapping(ui.buttonApply_3,3);
		connect(ui.buttonApply_3,SIGNAL(clicked()),&applyMapper,SLOT(map()));
		connect(&applyMapper,SIGNAL(mapped(int)),this,SLOT(applySettings(int)));

		connect(ui.buttonDone,SIGNAL(clicked()),this,SLOT(close()));
		
		startupWidget = new QWidget(this);
		startupLayout = new DynamicPluginLayout(startupWidget);
		startupWidget->setLayout(startupLayout);
		startupWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
		ui.startupScrollArea->setWidget(startupWidget);
		

		favoritesWidget = new QWidget(this);
		favoritesLayout = new DynamicPluginLayout(favoritesWidget);
		favoritesWidget->setLayout(favoritesLayout);
		favoritesWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
		ui.favoritesScrollArea->setWidget(favoritesWidget);
		
	}
	void setCurrentQSettingsPtr(QSettings* p){settings=p;}
	void setPluginTypes(QStringList plugins)
	{
		startupCheckboxes.clear();
		favoritesCheckboxes.clear();
		for(int i=0; i<plugins.size();++i)
		{
			Checkbox* item = new Checkbox(plugins.at(i),0);
			startupCheckboxes.push_back(item);

			item = new Checkbox(plugins.at(i),0);
			favoritesCheckboxes.push_back(item);
		}
		startupLayout->setup(startupCheckboxes);
		favoritesLayout->setup(favoritesCheckboxes);
	}
	void refreshSettings(){}
public slots:
	void applySettings(int page)
	{
		printf("apply %i clicked\n",page);
		switch(page)
		{
		case 1: applyPageSettings1(); break;
		case 2: applyPageSettings2(); break;
		case 3: applyPageSettings3(); break;
		}
	}
private:
	QSettings* settings;
	QSignalMapper signalMapper;
	QSignalMapper applyMapper;
	Ui::SettingsEditor ui;
	QWidget* startupWidget;
	QWidget* favoritesWidget;
	DynamicPluginLayout* startupLayout;
	DynamicPluginLayout* favoritesLayout;
	QVector<Checkbox*> favoritesCheckboxes;
	QVector<Checkbox*> startupCheckboxes;

	QVector<QString> settingsStrings;
	
	void applyPageSettings1(){}
	void applyPageSettings2()
	{
		QVector<QString> checkedItems;
		for(int i=0;i<favoritesCheckboxes.size();++i)
		{
			Checkbox* cb = favoritesCheckboxes.at(i);
			if(cb && cb->isChecked()) 
			{
				checkedItems.push_back(cb->text);
				printf("checked item: %s\n",cb->text.toAscii().constData());
			}
		}
	}
	void applyPageSettings3(){}
	void loadPageSettings1(QString str){}
	void loadPageSettings2(QString str){}
	void loadPageSettings3(QString str){}
	void loadSettings()
	{
	}
};
class UIDock : public QDockWidget
{
	Q_OBJECT
public:
	UIDock(QString id):myID(id),floatingStatus(false)
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
public slots:
	void dockFloatingChanged(bool floating){floatingStatus = floating;emit dockStatusChanged(myID,floating);}
	void showDock(bool b){setVisible(b);}
	void floatDock(bool b){setFloating(b);}
	void requestQuit(){emit quit(myID);}
signals:
	void dockStatusChanged(QString id, bool floating);
	void dockShowStatusChanged(bool b);
	void quit(QString id);
	
protected:
	QTabWidget* tabWidget;
	QAction* myAction;
	QMenu* myContextMenu;
	QString myID;
	bool floatingStatus;
};