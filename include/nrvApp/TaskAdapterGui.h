#pragma once
#include "ui_TaskAdapterGui.h"
#include "nrvToolbox/RebroadcastQtAdapter.h"
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QLineEdit>
#include <QtGui/QFileDialog>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtCore/QSignalMapper>
#include "nrv/GuiTools.h"
#include "nrvToolbox/Rebroadcast.h"

class TaskAdapter;
class PreviewWindow : public QMainWindow
{
Q_OBJECT
public:
	PreviewWindow(QDialog* centralWidget, QWidget* parent, QString str, bool file) : QMainWindow(parent,Qt::Window)
	{
		ui = centralWidget;
		QMenuBar* menubar = new QMenuBar(this);
		setMenuBar(menubar);
		QMenu* menu = menubar->addMenu("Options");
		QAction* loadAction = menu->addAction("Load");
		QAction* saveAction = menu->addAction("Save as");
		QAction* applyAction = menu->addAction("Apply");
		connect(applyAction,SIGNAL(triggered()),this,SLOT(apply()));
		connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));
		connect(loadAction,SIGNAL(triggered()),this,SLOT(load()));
		
		ui->setMinimumSize(ui->size());
		ui->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
		setCentralWidget(ui);

		if(!str.isEmpty())
		{
			if(file)GuiToolsClass::readConfigFile(ui,str.toStdString());
			else    GuiToolsClass::readConfigQString(ui, &str); 
		}
	}
public slots:
	void apply(){emit stringSet(GuiToolsClass::writeConfigQString(ui));}
	void save()
	{
		QString filename = QFileDialog::getSaveFileName(this, QString("Save as..."),
                            QString(),
                            QString("NERVE config files (*.ncf);;Text files (*.txt)"));
		if(!filename.isEmpty())
		{
			GuiToolsClass::writeConfigFile(ui, filename.toStdString());
		}
	}
	void load()
	{
		QString filename = QFileDialog::getOpenFileName(this, QString("Select a file to load..."),
							QString(),
							QString("NERVE config files (*.ncf);;Text files (*.txt)"));
		if(!filename.isEmpty())
		{
			GuiToolsClass::readConfigFile(ui,filename.toStdString());
		}
	}
signals:
	void stringSet(QString);

private:
	QDialog* ui;
};
struct RendererSceneInfo
{
	RendererSceneInfo(std::string _provider, std::string _service, int _index):provider(_provider),service(_service),index(_index){}
	std::string provider;
	std::string service;
	int index;
};
class RendererButtons : public QWidget
{
	Q_OBJECT
public:
	RendererButtons(QString provider,QString service,unsigned int service_id,int numScenes, int sceneRootIndex, QWidget* parent = 0):
	  p_id(provider),
	  s_id(service),
	  serviceID(service_id),
	  index(sceneRootIndex)
	{
		QHBoxLayout* layout = new QHBoxLayout(this);
		QLabel* label = new QLabel(this);
		label->setMargin(0);
		//label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
		label->setText(p_id + ": " + s_id);
		//label->setWordWrap(true);
		layout->addStretch(1);
		layout->addWidget(label);
		layout->setMargin(0);
		layout->setContentsMargins(0,0,0,0);
		for(int i=0;i<numScenes;++i)
		{
			QCheckBox* cb = new QCheckBox(this);
			cb->setContentsMargins(0,0,0,0);
			checkBoxes.push_back(cb);
			layout->addWidget(cb);
			signalMapper.setMapping(cb,i);
			connect(cb,SIGNAL(stateChanged(int)),&signalMapper,SLOT(map()));			
		}
		setFixedHeight(18);
		adjustSize();
		//QRect interior = contentsRect();
		//printf("%RendererButtons size %i %i\n",interior.width(),interior.height());
		//setFixedSize(width(),height());
		connect(&signalMapper,SIGNAL(mapped(int)),this,SLOT(checkboxChanged(int)));
	}
signals:
	  void rendererSelected(QString provider,QString service,int index,bool checked);
public slots:
	void checkboxChanged(int i)
	{
		bool state = checkBoxes.at(i)->isChecked();
		//if the button is checked, uncheck the others
		if(state==true)
		{
			for(int index = 0;index<checkBoxes.size();++index)
			{
				if(index!=i)
				{
					QCheckBox* cb = checkBoxes.at(index);
					cb->blockSignals(true);
					cb->setChecked(false);
					cb->blockSignals(false);
				}
			}
		}
		emit rendererSelected(p_id,s_id,i,state);
	}
private:
	QString p_id;
	QString s_id;
	QSignalMapper signalMapper;
	QVector<QCheckBox*> checkBoxes;
	int index;
	unsigned int serviceID;
};

class TaskAdapterGui : public QDialog
{
Q_OBJECT
public:
	TaskAdapterGui();
	~TaskAdapterGui(){}
	void preview(QDialog* ui,std::string str, bool file)
	{
		PreviewWindow* p = new PreviewWindow(ui,this,QString(str.c_str()),file);
		connect(p,SIGNAL(stringSet(QString)),this,SLOT(setPreviewString(QString)));
		p->show();
	}
	void addRenderer(QString provider,QString service, unsigned int serviceID, int numScenes,int index)
	{
		RendererButtons* r = new RendererButtons(provider,service,serviceID,numScenes,index,this);
		rendererButtonsMap.insert(serviceID, r);
		QVBoxLayout* layout = static_cast<QVBoxLayout*>(ui.scrollArea->widget()->layout());
		layout->insertWidget(layout->count()-1,r,0,Qt::AlignRight);		
		connect(r,SIGNAL(rendererSelected(QString,QString,int,bool)),this,SLOT(rendererSelected(QString,QString,int,bool)));
	}
	void removeRenderer(unsigned int serviceID)
	{
		QMap<unsigned int,RendererButtons*>::iterator iter = rendererButtonsMap.find(serviceID);
		if(iter != rendererButtonsMap.end())
		{
			RendererButtons* r = (*iter);
			delete *iter;
			rendererButtonsMap.erase(iter);
		}
		
	}
	void setSceneRootList(std::vector<std::string> names)
	{
		ui.sceneRootList->clear();
		for(unsigned int i=0;i<names.size();++i)
		{
			QString str = QString::number((int)i);
			str.append(": ").append(QString::fromStdString(names[i]));
			ui.sceneRootList->addItem(str);
		}
	}
public slots:
	void update();
	void browseButtonClicked();
	void setPreviewString(QString str)
	{
		connect(previewStringLineEdit,SIGNAL(textChanged(QString)),previewStringLineEdit,SIGNAL(returnPressed()));
		previewStringLineEdit->setText(str);
		disconnect(previewStringLineEdit,SIGNAL(textChanged(QString)),previewStringLineEdit,SIGNAL(returnPressed()));
	}
	void rendererSelected(QString provider,QString service,int index,bool checked)
	{
		int sceneRootIndex = index;
		if(checked==false) index = -1;
		RendererSceneInfo info(provider.toStdString(),service.toStdString(),index);
		rendererInfoRebroadcaster->signal_toOtherThreads.broadcast(info);
	}
private:
	RebroadcastQtAdapter rebroadcastAdapter;
	RebroadcastManager   rebroadcastManager;
	Rebroadcaster<RendererSceneInfo>* rendererInfoRebroadcaster;
	Ui::TaskAdapterGui ui;
	QLineEdit* previewStringLineEdit;
	QMap<unsigned int,RendererButtons*> rendererButtonsMap;
	friend class TaskAdapter;
};