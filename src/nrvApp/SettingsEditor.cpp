#include "nrvApp/SettingsEditor.h"

SettingsEditor::SettingsEditor(QSettings* s, QWidget* parent)
	{
		settings=s;
		ui.setupUi(this);
		ui.stackedWidget->setCurrentIndex(0);

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

		connect(ui.buttonDone,SIGNAL(clicked()),this,SLOT(close()));
				
		startupPage = new StartupPage(this);
		startupPage->setGroupString("Startup");
		startupPage->setDefaultQSettings(settings);
		startupPage->setScrollArea(ui.startupScrollArea);
		startupPage->setStatusText(ui.Status_Text);
		startupPage->setApplyButtonPtr(ui.buttonApply);
	}
QString SettingsEditor::getStartupPlugin()
{
	StartupPage* s = dynamic_cast<StartupPage*>(startupPage);
	if(s)	return s->getCurrentSelection();
	else return QString();
}