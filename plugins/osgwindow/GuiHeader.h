#include "ui_osgwindow.h"
#include <QIntValidator>
#include <QString>
#include "osgwindow_guts.h"

class osgwindowGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	osgwindowGui(Window* w):window(w)
	{
		ui.setupUi(this);
		connect(ui.windowed,SIGNAL(clicked()),this,SLOT(windowed()));
		connect(ui.fullscreen,SIGNAL(clicked()),this,SLOT(fullscreen()));
	}
public slots:
	void windowed(){window->setupWindowed();}
	void fullscreen(){window->setupFullscreen();}
protected:
	Ui::osgwindowDlg ui;
	Window* window;
};