#include "nrvApp/TaskAdapterGui.h"
#include "nrvApp/TaskAdapter.h"
#include <QtCore/QTimer>
#include <QtGui/QFileDialog>
TaskAdapterGui::TaskAdapterGui()
{
	ui.setupUi(this);
	QTimer* timer = new QTimer(this);
	timer->start(200);
	previewStringLineEdit = new QLineEdit(this);
	previewStringLineEdit->setObjectName("previewStringLineEdit");
	previewStringLineEdit->setVisible(false);
	connect(timer,SIGNAL(timeout()),this,SLOT(update()));
	connect(ui.browseButton,SIGNAL(clicked()),this,SLOT(browseButtonClicked()));
	QWidget* widget = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(widget);
	ui.scrollArea->setWidget(widget);
	widget->setLayout(layout);
	widget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
	layout->addStretch(0);
	
	rebroadcastAdapter.setRebroadcastManager(&rebroadcastManager);
	rebroadcastAdapter.addWithChildren(this);//must happen after the rebroadcast manager is set
	rendererInfoRebroadcaster = new Rebroadcaster<RendererSceneInfo>();
	rebroadcastManager.add(rendererInfoRebroadcaster, "RendererInfo");
	
}

void TaskAdapterGui::update()
{
	rebroadcastManager.rebroadcastAll();
	if(ui.taskActive->value()==0) ui.launchButton->setText("Launch task");
	else                          ui.launchButton->setText("Cancel task");
	if(ui.taskRunning->value()==0)ui.runButton->setText("Run");
	else						  ui.runButton->setText("Pause");
}
void TaskAdapterGui::browseButtonClicked()
{
	QString old_str = ui.configFileLineEdit->text();
	QString fileName = QFileDialog::getOpenFileName(this, QString("Save as..."),
                            QString(),
                            QString("Task config files (*.ncf);;Text files (*.txt)"));
	if(old_str.compare(fileName) != 0)
	{
		QObject::connect(ui.configFileLineEdit, SIGNAL(textChanged(QString)),ui.configFileLineEdit,SIGNAL(returnPressed()));
		ui.configFileLineEdit->setText(fileName);
		QObject::disconnect(ui.configFileLineEdit, SIGNAL(textChanged(QString)),ui.configFileLineEdit,SIGNAL(returnPressed()));
	}

}
