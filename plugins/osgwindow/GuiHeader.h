#pragma once
#include "ui_osgwindow.h"
#include <QIntValidator>
#include <QString>
#include <QButtonGroup>
#include "osgwindow_guts.h"
#include <QFileDialog>

class osgwindowGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	osgwindowGui(Window* w):window(w)
	{
		ui.setupUi(this);
		QButtonGroup* bg=new QButtonGroup(this);
		bg->addButton(ui.windowed);
		bg->addButton(ui.fullscreen);
		bg->setExclusive(true);
		connect(ui.applyButton,SIGNAL(clicked()),this,SLOT(setupWindow()));
		connect(ui.loadButton,SIGNAL(clicked()),this,SLOT(load()));
		ui.cameraManipulation->addItem("Custom Callback");
		ui.cameraManipulation->addItem("Custom Manipulator");
		ui.cameraManipulation->addItem("Trackball");
		ui.cameraManipulation->addItem("First Person");
	}
	~osgwindowGui(){printf("osgwindow gui dtor\n");}
public slots:
	void setupWindow()
	{
		WindowParams wp;
		wp.windowed = ui.windowed->isChecked();
		wp.mirrored = ui.mirrored->isChecked();
		wp.stereo   = ui.stereo->isChecked();
		wp.screenNum = ui.screenNum->value();
		wp.frameRate = ui.frameRate->value();
		wp.camManipOption = ui.cameraManipulation->currentIndex();
		window->configure(wp);
	}
	void load()
	{
		QString file = QFileDialog::getOpenFileName(0,"Select file to view","./","*.osg*");
		window->loadFile(file.toStdString());
	}
protected:
	Ui::osgwindowDlg ui;
	Window* window;
};