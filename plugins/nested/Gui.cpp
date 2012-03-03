#include "Gui.h"
#include "TutorialPlugin.h"
#include <QThread>

void TutorialGui::create()
{
	QString s = ui.comboBox->currentText();
	QString child;
	if(!s.isEmpty()) plugin->create(s.toStdString());
}
void TutorialGui::refreshAvailable()
{
	plugin->refreshAvailable();
}
void TutorialGui::toggleOwnership(bool b)
{
	plugin->toggleTakeOwnership(b);
}
void TutorialGui::toggleChildUIs(bool b)
{
	plugin->toggleHandleChildUIs(b);
}
void TutorialGui::kill()
{
	QString s= childModel.data(ui.listView->currentIndex(),Qt::DisplayRole).toString();
	plugin->cancelChild(s.toStdString());
	childList.removeOne(s);
	childModel.setStringList(childList);
}
void TutorialGui::detach()
{
	QString s= childModel.data(ui.listView->currentIndex(),Qt::DisplayRole).toString();
	plugin->detachChild(s.toStdString());
	childList.removeOne(s);
	childModel.setStringList(childList);
}

