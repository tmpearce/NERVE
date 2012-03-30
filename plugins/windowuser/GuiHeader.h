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
	}
	void acceptChildUI(QWidget* child)
	{
		ui.viewerScrollArea->setWidget(child);
		//ui.tabWidget->setFixedSize(ui.tabWidget->minimumSizeHint());
	}
public slots:
	void on_startButton_clicked();
protected:
	TutorialPlugin* plugin;
	Ui::TutorialDialog ui;
};