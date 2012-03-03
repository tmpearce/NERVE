#pragma once
#include "ui_nested.h"
#include <QIntValidator>
#include <QString>
#include <QStringListModel>
#include <QTimer>
#include <QThread>
class TutorialPlugin;

class TutorialGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	TutorialGui(TutorialPlugin* p):plugin(p)
	{
		ui.setupUi(this);
		ui.listView->setModel(&childModel);
		ui.comboBox->setModel(&pluginModel);
		connect(ui.create,SIGNAL(clicked()),this,SLOT(create()));
		connect(ui.refresh,SIGNAL(clicked()),this,SLOT(refreshAvailable()));
		connect(ui.ownershipBox,SIGNAL(toggled(bool)),this,SLOT(toggleOwnership(bool)));
		connect(ui.childUIBox,SIGNAL(toggled(bool)),this,SLOT(toggleChildUIs(bool)));
		connect(ui.killButton,SIGNAL(clicked()),this,SLOT(kill()));
		connect(ui.detachButton,SIGNAL(clicked()),this,SLOT(detach()));
	}
	void addPlugin(std::string name)
	{
		childList.push_back(QString::fromStdString(name));
		childModel.setStringList(childList);
	}
	void setAvailablePlugins(std::vector<std::string> l)
	{
		pluginList.clear();
		for(int i=0;i<l.size();++i)
		{
			pluginList.push_back(QString::fromStdString(l.at(i)));
		}
		pluginModel.setStringList(pluginList);
	}
public slots:
	void create();
	void refreshAvailable();
	void toggleOwnership(bool);
	void toggleChildUIs(bool);
	void kill();
	void detach();
protected:
	TutorialPlugin* plugin;
	Ui::TutorialDialog ui;
	QStringListModel childModel;
	QStringListModel pluginModel;
	QStringList childList;
	QStringList pluginList;
};

class A
{
public:
	A(int i){}
};
class B : A
{
public:
	B(int i):A(i*5){}
};