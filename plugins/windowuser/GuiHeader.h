#pragma once
#include "ui_windowuser.h"
#include <QIntValidator>
#include <QString>
#include <QTabWidget>
#include <QLayout>

class TutorialPlugin;

class TutorialGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	TutorialGui(TutorialPlugin* p):plugin(p)
	{
		ui.setupUi(this);
		connect(ui.createWindow,SIGNAL(clicked()),this,SLOT(createWindow()));
		//ui.tabWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	}
	void acceptChildUI(QWidget* child)
	{
		ui.tabWidget->addTab(child,"child ui");
		ui.tabWidget->setFixedSize(ui.tabWidget->minimumSizeHint());
	}
public slots:
	void createWindow();
protected:
	TutorialPlugin* plugin;
	Ui::TutorialDialog ui;
};