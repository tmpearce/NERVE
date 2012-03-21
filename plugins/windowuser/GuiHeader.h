#include "ui_windowuser.h"
#include <QIntValidator>
#include <QString>
#include <QTabWidget>

class TutorialGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	TutorialGui()
	{
        
		ui.setupUi(this);
				
	}
	void acceptChildUI(QWidget* child)
	{
		ui.tabWidget->addTab(child,"child ui");
	}
public slots:
	
protected:
	
	Ui::TutorialDialog ui;
};