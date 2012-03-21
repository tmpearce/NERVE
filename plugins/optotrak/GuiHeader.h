#pragma once
#include "ui_Optotrak.h"


class OptotrakGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	OptotrakGui():connectedStatus(false)
	{
		ui.setupUi(this);
		connect(ui.connectButton,SIGNAL(clicked()),this,SLOT(connectOptotrak()));
	}
public slots:
	void connectOptotrak()
	{
		if(isConnected()) return;
		//(re)establish connection or call function that does
	}
protected:
	Ui::OptotrakGui ui;
	bool connectedStatus;

	void setConnectedStatus(bool status)
	{
		connectedStatus=status;
		ui.connectedIndicator->setValue(100*(int)connectedStatus);
	}
	bool isConnected(){return connectedStatus;}
};