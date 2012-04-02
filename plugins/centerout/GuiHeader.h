#pragma once
#include "ui_centerout_example.h"
#include <QIntValidator>
#include <QString>
#include <QTabWidget>
#include <QLayout>

class CenterOutPlugin;

class CenterOutGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	CenterOutGui(CenterOutPlugin* p):plugin(p)
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
	CenterOutPlugin* plugin;
	Ui::TutorialDialog ui;
};