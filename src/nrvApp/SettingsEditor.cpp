#include "nrvApp/SettingsEditor.h"

SettingsEditor::SettingsEditor(QWidget* parent)
	{
		ui.setupUi(this);

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
		
		/*applyMapper.setMapping(ui.buttonApply,1);
		connect(ui.buttonApply,SIGNAL(clicked()),&applyMapper,SLOT(map()));
		applyMapper.setMapping(ui.buttonApply_2,2);
		connect(ui.buttonApply_2,SIGNAL(clicked()),&applyMapper,SLOT(map()));
		applyMapper.setMapping(ui.buttonApply_3,3);
		connect(ui.buttonApply_3,SIGNAL(clicked()),&applyMapper,SLOT(map()));
		connect(&applyMapper,SIGNAL(mapped(int)),this,SLOT(applySettings(int)));*/

		connect(ui.buttonDone,SIGNAL(clicked()),this,SLOT(close()));
		
		//startupWidget = new QWidget(this);
		//startupLayout = new DynamicPluginLayout(startupWidget);
		//startupWidget->setLayout(startupLayout);
		//startupWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
		//ui.startupScrollArea->setWidget(startupWidget);
		

		favoritesWidget = new QWidget(this);
		favoritesLayout = new DynamicPluginLayout(favoritesWidget);
		favoritesWidget->setLayout(favoritesLayout);
		favoritesWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
		ui.favoritesScrollArea->setWidget(favoritesWidget);
		
		startupPage = new StartupPage(this);
		startupPage->setScrollArea(ui.startupScrollArea);
	}