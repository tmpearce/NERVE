#pragma once
#include "nrvApp/_UIC_SettingsEditor.h"
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
	virtual void setApplyButtonPtr(QPushButton* b)=0;
	virtual void setScrollArea(QScrollArea* )=0;
	virtual void refresh()=0;
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
	SettingsEditor(QWidget* parent);
	void setCurrentQSettingsPtr(QSettings* p){settings=p;}
	void setPluginTypes(QStringList plugins)
	{
		pluginTypes = plugins;
		//startupCheckboxes.clear();
		favoritesCheckboxes.clear();
		for(int i=0; i<plugins.size();++i)
		{
			//Checkbox* item = new Checkbox(plugins.at(i),0);
			//startupCheckboxes.push_back(item);

			Checkbox* item = new Checkbox(plugins.at(i),0);
			favoritesCheckboxes.push_back(item);
		}
		//startupLayout->setup(startupCheckboxes);
		favoritesLayout->setup(favoritesCheckboxes);

		startupPage->refresh();
	}
	QStringList getPluginTypes(){return pluginTypes;}
	void refreshSettings(){}
	void saveSettings()
	{
		printf("saveSettings\n");
		QString path = settings->value("main/save_config_path", QDir::current().path() ).toString();
		QFileDialog fd(this,"Save config as...",path,"ini files(*.ini)");
		fd.setAcceptMode(QFileDialog::AcceptSave);
		if(fd.exec())
		{
			QDir dir = fd.directory();
			settings->setValue("main/save_config_path",dir.absolutePath());
			QStringList s = fd.selectedFiles();
			printf("%s **/** %s\n",dir.absolutePath().toAscii().constData(),s.first().toAscii().constData());
			
		}
	}
	void loadSettings(){printf("loadSettings\n");}
public slots:
	/*void applySettings(int page)
	{
		printf("apply %i clicked\n",page);
		switch(page)
		{
		case 1: applyPageSettings1(); break;
		case 2: applyPageSettings2(); break;
		case 3: applyPageSettings3(); break;
		}
	}*/
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
	//QVector<Checkbox*> startupCheckboxes;
	QStringList pluginTypes;
	SettingsPage* startupPage;
	//QVector<QString> settingsStrings;
	
	/*void applyPageSettings1(){}
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
	void loadPageSettings3(QString str){}*/
	
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
	}
	void save(QSettings* settings){}
	void load(QSettings* settings){}
	void setApplyButtonPtr(QPushButton* b)
	{
		connect(b,SIGNAL(clicked()),this,SLOT(apply()));
	}
	void setScrollArea(QScrollArea* sa)
	{
		myWidget->setParent(sa);
		sa->setWidget(myWidget);
	}
	void refresh()
	{
		QStringList plugins = parent->getPluginTypes();
		QStringList::iterator iter = plugins.begin();
		myLayout->clear();
		for(;iter!=plugins.end();++iter)
		{
			QString pname = *iter;
			Checkbox* c = new Checkbox(pname);
			myLayout->add(c);
		}
		myLayout->complete();
	};
public slots:
	void apply(){}
private:
	SettingsEditor* parent;
	QWidget* myWidget;
	DynamicPluginLayout* myLayout;
	QString currentSelection;
	QSignalMapper mapper;
	QButtonGroup buttonGroup;
};