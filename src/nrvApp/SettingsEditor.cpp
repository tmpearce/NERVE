#include "nrvApp/SettingsEditor.h"

SettingsEditor::SettingsEditor(QSettings* s, QWidget* parent)
	{
		settings=s;
		ui.setupUi(this);
		pp.setup(ui,settings);
		ui.stackedWidget->setCurrentWidget(ui.menuPage);

		//Set up the mappings from buttons on the menuPage to the appropriate widgets
		connect(&signalMapper,SIGNAL(mapped(QWidget*)),ui.stackedWidget,SLOT(setCurrentWidget(QWidget*)));
		signalMapper.setMapping(ui.buttonStartup,ui.startupPage);
		connect(ui.buttonStartup,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonFavorites,ui.favoritesPage);
		connect(ui.buttonFavorites,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonMain_GUI,ui.mainGuiPage);
		connect(ui.buttonMain_GUI,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonPluginPath,ui.pluginPathPage);
		connect(ui.buttonPluginPath,SIGNAL(clicked()),&signalMapper,SLOT(map()));

		signalMapper.setMapping(ui.buttonReturn,ui.menuPage);
		connect(ui.buttonReturn,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonReturn_2,ui.menuPage);
		connect(ui.buttonReturn_2,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonReturn_3,ui.menuPage);
		connect(ui.buttonReturn_3,SIGNAL(clicked()),&signalMapper,SLOT(map()));
		signalMapper.setMapping(ui.buttonReturn_4,ui.menuPage);
		connect(ui.buttonReturn_4,SIGNAL(clicked()),&signalMapper,SLOT(map()));

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
void SettingsEditor::applyPluginPath()
{
}