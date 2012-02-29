#include "Gui.h"
#include "TutorialPlugin.h"

void TutorialGui::create()
{
	QString s = ui.comboBox->currentText();
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