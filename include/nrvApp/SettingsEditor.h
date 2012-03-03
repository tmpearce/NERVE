#pragma once
#include "ui_SettingsEditor.h"
#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QVBoxLayout>
#include <QtGui/QCheckBox>
#include <QtCore/QSignalMapper>
#include <QtGui/QPushButton>

class SettingsPage : public QWidget
{
public:
	virtual void save(QSettings* settings)=0;
	virtual void load(QSettings* settings)=0;
	virtual void setDefaultQSettings(QSettings* settings)=0;
	virtual void setApplyButtonPtr(QPushButton* b)=0;
	virtual void setScrollArea(QScrollArea* )=0;
	virtual void setStatusText(QLabel* )=0;
	virtual void setGroupString(const QString&)=0;
	virtual void refresh()=0;
};
class PluginPath : public QWidget
{
	Q_OBJECT
public:
	PluginPath(QWidget* parent = 0):QWidget(parent),useAbsolutePath(false),includeSubDirs(false){}
	void setup(Ui::SettingsEditor& Ui,QSettings* s)
	{
		settings = s;
		ui=&Ui;
		ui->useAbsolutePath->setChecked(true);//always use absolute path
		ui->useAbsolutePath->setEnabled(false);//disable, not implemented yet
		pathAbsolute = settings->value("PluginPath/pathAbsolute").toString();
		ui->currentPath->setText(pathAbsolute);
		includeSubDirs = settings->value("PluginPath/includeSubDirs").toBool();
		ui->includeSubDirs->setChecked(includeSubDirs);
		connect(ui->buttonApply_4,SIGNAL(clicked()),this,SLOT(apply()));
		connect(ui->editPath,SIGNAL(clicked()),this,SLOT(editPath()));
		connect(ui->useAbsolutePath,SIGNAL(toggled(bool)),this,SLOT(useAbsPathToggled()));
		connect(ui->includeSubDirs,SIGNAL(toggled(bool)),this,SLOT(useSubDirsToggled()));
		connect(ui->currentPath,SIGNAL(textChanged(const QString&)),this,SLOT(textChanged()));
	}
	std::string currentPath(){return pathAbsolute.toStdString();}
	bool useSubDirs(){return includeSubDirs;}
public slots:
	void editPath()
	{
		QFileDialog fd(this,"Select plugin path...",pathAbsolute);
		fd.setOption(QFileDialog::ShowDirsOnly);
		fd.setFileMode(QFileDialog::DirectoryOnly);
		if(fd.exec())
		{
			QDir dir = fd.directory();
			ui->currentPath->setText(dir.path());
			
		}
	}
	void textChanged(){ui->pluginPathPageStatus->setText("Settings changed but not applied");}
	void useAbsPathToggled(){ui->pluginPathPageStatus->setText("Settings changed but not applied");}
	void useSubDirsToggled(){ui->pluginPathPageStatus->setText("Settings changed but not applied");}
	void apply()
	{
		pathAbsolute = ui->currentPath->text();
		includeSubDirs = ui->includeSubDirs->isChecked();
		ui->pluginPathPageStatus->setText("");
		settings->setValue("PluginPath/pathAbsolute",pathAbsolute);
		settings->setValue("PluginPath/includeSubDirs",includeSubDirs);
	}
protected:
	QString pathAbsolute;
	QString pathRelative;
	bool useAbsolutePath;
	bool includeSubDirs;
	Ui::SettingsEditor* ui;
	QSettings* settings;
};

class SettingsEditor : public QDialog
{
	Q_OBJECT
public:
	SettingsEditor(QSettings* s, QWidget* parent);
	void setPluginTypes(QStringList plugins)
	{
		pluginTypes = plugins;
		startupPage->refresh();
	}
	QStringList getPluginTypes(){return pluginTypes;}
	void saveSettingsAs()
	{
		QString path = settings->value("main/save_config_path", QDir::current().path() ).toString();
		QFileDialog fd(this,"Save config as...",path,"ini files(*.ini)");
		fd.setAcceptMode(QFileDialog::AcceptSave);
		if(fd.exec())
		{
			QDir dir = fd.directory();
			settings->setValue("main/save_config_path",dir.absolutePath());
			QStringList s = fd.selectedFiles();
			printf("%s\n",s.first().toAscii().constData());
			
		}
	}
	void loadSettings()
	{
		startupPage->load(settings);
	}
	QString getStartupPlugin();
	std::pair<std::string,bool> getPluginPathInfo(){return std::pair<std::string,bool>(pp.currentPath(),pp.useSubDirs());}
public slots:
	void applyPluginPath();
private:
	Ui::SettingsEditor ui;
	PluginPath pp;
	QSettings* settings;
	QSignalMapper signalMapper;	
	QStringList pluginTypes;
	SettingsPage* startupPage;
	
};
class Checkbox : public QWidget
{
	Q_OBJECT
public:
	Checkbox(const QString& text_, QWidget* parent=0):text(text_),QWidget(parent)
	{
		QHBoxLayout* layout = new QHBoxLayout(this);
		qcheckbox = new QCheckBox(this);
		QLabel* label = new QLabel(text,this);
		label->setWordWrap(true);			
		layout->addWidget(qcheckbox);
		layout->addWidget(label);

		connect(qcheckbox,SIGNAL(toggled(bool)),this,SLOT(wasChecked(bool)));
	}
	~Checkbox(){}
	bool isChecked(){return qcheckbox->isChecked();}
	QString text;
	QCheckBox* qcheckbox;
signals:
	void checked(const QString&);
public slots:
	void wasChecked(bool b){if(b) emit checked(text);}
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
	void add(Checkbox* c)
	{
		addWidget(c,0,Qt::AlignLeft);
	}
	void complete(){addStretch(0);}
};
class StartupPage : public SettingsPage
{
	Q_OBJECT	
public:
	StartupPage(SettingsEditor* se):parent(se)
	{
		myWidget = new QWidget(0);
		myLayout = new DynamicPluginLayout(myWidget);
		myWidget->setLayout(myLayout);
		myWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
		buttonGroup = new QButtonGroup(this);

		statusText=0;
		defaultSettings=0;
	}
	void save(QSettings* settings)//the group option on the QSettings must be appropriately set before this call
	{
		settings->setValue("StartupPluginText", savedSelection);
	}
	void load(QSettings* settings)
	{
		settings->beginGroup(groupString);
		currentSelection= settings->value("StartupPluginText").toString();
		settings->endGroup();
		refresh();//updates valid plugin options, and selects saved option (if it exists)
	}
	void setGroupString(const QString& str){groupString=str;}
	void setDefaultQSettings(QSettings* settings){defaultSettings=settings;}
	void setApplyButtonPtr(QPushButton* b)
	{
		connect(b,SIGNAL(clicked()),this,SLOT(apply()));
	}
	void setScrollArea(QScrollArea* sa)
	{
		myWidget->setParent(sa);
		sa->setWidget(myWidget);
	}
	void setStatusText(QLabel* l)
	{
		statusText=l;
	}
	void refresh()
	{
		QStringList plugins = parent->getPluginTypes();
		QStringList::iterator iter = plugins.begin();
		myLayout->clear();
		delete buttonGroup;
		buttonGroup = new QButtonGroup(this);
		Checkbox* n = new Checkbox("Don't load a plugin on startup");
		myLayout->add(n);
		buttonGroup->addButton(n->qcheckbox);
		n->qcheckbox->setChecked(true);
		connect(n,SIGNAL(checked(QString)),this,SLOT(selectionChanged(QString)));
		for(;iter!=plugins.end();++iter)
		{
			QString pname = *iter;
			Checkbox* c = new Checkbox(pname);
			myLayout->add(c);
			buttonGroup->addButton(c->qcheckbox);
			connect(c,SIGNAL(checked(QString)),this,SLOT(selectionChanged(QString)));
			if(QString::localeAwareCompare(pname,currentSelection)==0)
			{
				c->qcheckbox->setChecked(true);
			}
		}
		myLayout->complete();
		statusText->setText("");
	};
	QString getCurrentSelection()
	{
		QString str;
		if(QString::localeAwareCompare(currentSelection,"Don't load a plugin on startup") != 0)
		{
			str=currentSelection;
		}
		return str;
	}
public slots:
	void apply()
	{
		savedSelection=currentSelection;
		if(defaultSettings!=0) 
		{
			defaultSettings->beginGroup(groupString);
			defaultSettings->setValue("StartupPluginText",savedSelection);
			defaultSettings->endGroup();
			statusText->setText("Selection saved");
		}
		else
		{
			statusText->setText("Error saving: default QSettings* is NULL");
		}
		
	}
	void selectionChanged(const QString& str)
	{
		currentSelection = str;
		statusText->setText("Press Apply to save selection");
	}
private:
	SettingsEditor* parent;
	QWidget* myWidget;
	DynamicPluginLayout* myLayout;
	QString currentSelection;
	QString savedSelection;
	QString groupString;
	QSignalMapper mapper;
	QButtonGroup* buttonGroup;
	QLabel* statusText;
	QSettings* defaultSettings;
};