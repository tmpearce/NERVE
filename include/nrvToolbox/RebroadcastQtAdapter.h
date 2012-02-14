#pragma once
#include "nrvToolbox/RebroadcastManager.h"
#include "math.h"

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>


class QtRebroadcastVoid: public QObject, public UsesSlots
{
	Q_OBJECT
public:
	QtRebroadcastVoid(QObject* obj = NULL)
	{
		guiObject = NULL;
		if (obj!=NULL)
			setQObject(obj);
	};
	void setQObject(QObject* obj)
	{
		if ( dynamic_cast<QPushButton*>(obj) )
		{
			guiObject = obj;
			connect(dynamic_cast<QPushButton*>(guiObject), SIGNAL(clicked()), this, SLOT(slot_connectToQSignal()));
		}
		else 
			printf("ERROR: Attempted to add QObject: %s to a QtRebroadcastVoid, but this type of QObject is not registered.\n");
	}
public slots:
	void slot_connectToQSignal()
	{
		signal_toRebroadcast.broadcast();
	}
signals:
	void signal_toQSlot();
public:
	void slot_connectToRebroadcastSignal()
	{
		guiObject->blockSignals(true);
		emit signal_toQSlot();
		guiObject->blockSignals(false);
	}
	signal0 signal_toRebroadcast;
	std::string getName()
	{
		if (guiObject!=NULL)
			return guiObject->objectName().toStdString();
		else
			return std::string("");
	};
	void sync()
	{
	}
protected:
	QObject * guiObject;
};
class QtRebroadcastDouble: public QObject, public UsesSlots
{
	Q_OBJECT
public:
	enum DoubleWidgetType
	{
		QCHECKBOX,
		QCOMBOBOX,
		QDOUBLESPINBOX,
		QPROGRESSBAR,
		QSPINBOX,
		QLCDNUMBER
	};
	QtRebroadcastDouble(QObject* obj = NULL)
	{
		guiObject = NULL;
		if (obj!=NULL)
			setQObject(obj);
	};
	void setQObject(QObject* obj)
	{
		if ( dynamic_cast<QCheckBox*>(obj) )
		{
			type = QCHECKBOX;
			guiObject = obj;
			// make connection from Qt
			connect(dynamic_cast<QCheckBox*>(guiObject), SIGNAL(toggled(bool)), this, SLOT(slot_connectToQSignal(bool)));
			// make connection to Qt
			connect(this, SIGNAL(signal_toQSlot(bool)), dynamic_cast<QCheckBox*>(guiObject), SLOT(setChecked(bool)));
		}
		else if ( dynamic_cast<QComboBox*>(obj) )
		{
			type = QCOMBOBOX;
			guiObject = obj;
			connect(dynamic_cast<QComboBox*>(guiObject), SIGNAL(currentIndexChanged(int)), this, SLOT(slot_connectToQSignal(int)));
			connect(this, SIGNAL(signal_toQSlot(int)), dynamic_cast<QComboBox*>(guiObject), SLOT(setCurrentIndex(int)));
		}
		else if ( dynamic_cast<QDoubleSpinBox*>(obj) )
		{
			type = QDOUBLESPINBOX;
			guiObject = obj;
			connect(dynamic_cast<QDoubleSpinBox*>(guiObject), SIGNAL(valueChanged(double)), this, SLOT(slot_connectToQSignal(double)));
			connect(this, SIGNAL(signal_toQSlot(double)), dynamic_cast<QDoubleSpinBox*>(guiObject), SLOT(setValue(double)));
		}
		else if ( dynamic_cast<QProgressBar*>(obj) )
		{		
			type = QPROGRESSBAR;
			guiObject = obj;
			connect(dynamic_cast<QProgressBar*>(guiObject), SIGNAL(valueChanged(int)), this, SLOT(slot_connectToQSignal(int)));
			connect(this, SIGNAL(signal_toQSlot(int)), dynamic_cast<QProgressBar*>(guiObject), SLOT(setValue(int)));
		}
		else if ( dynamic_cast<QSpinBox*>(obj) )
		{		
			type = QSPINBOX;
			guiObject = obj;
			connect(dynamic_cast<QSpinBox*>(guiObject), SIGNAL(valueChanged(int)), this, SLOT(slot_connectToQSignal(int)));
			connect(this, SIGNAL(signal_toQSlot(int)), dynamic_cast<QSpinBox*>(guiObject), SLOT(setValue(int)));
		}
		else if ( dynamic_cast<QLCDNumber*>(obj) )
		{		
			type = QLCDNUMBER;
			guiObject = obj;
			connect(this, SIGNAL(signal_toQSlot(double)), dynamic_cast<QLCDNumber*>(guiObject), SLOT(display(double)));
			connect(this, SIGNAL(signal_toQSlot(int)), dynamic_cast<QLCDNumber*>(guiObject), SLOT(display(int)));
		}
		else 
			printf("ERROR: Attempted to add QObject: %s to a QtRebroadcastDouble, but this type of QObject is not registered.\n", obj->objectName().toAscii().constData());
	}
	// This Qt slot receivs a signal from the QObject and re-broadcasts a thread-safe signal
public slots:
	void slot_connectToQSignal(double value)
	{
		signal_toRebroadcast.broadcast(value);
	}
	void slot_connectToQSignal(int value)
	{
		signal_toRebroadcast.broadcast((double)value);
	}
	void slot_connectToQSignal(bool value)
	{
		signal_toRebroadcast.broadcast((double)value);
	}
	signals:
	// This Qt signal is emmitted when a thread-safe signal indicates that the QObject value should be changed
	void signal_toQSlot(double newValue);
	void signal_toQSlot(int newValue);
	void signal_toQSlot(bool newValue);
public:
	// This non-Qt "slot" is called by a thread-safe signal in order to initiate the Qt signal
	void slot_connectToRebroadcastSignal(double value)
	{
		guiObject->blockSignals(true);
		if ( type == QLCDNUMBER )
		{
			if ( abs(value-floor(value)) < 0.01 || abs(value-ceil(value)) < 0.01 )
				emit signal_toQSlot((int)value);
			else
				emit signal_toQSlot(value);
		}
		else
		{
			emit signal_toQSlot(value);
			emit signal_toQSlot((int)value);
			emit signal_toQSlot((bool)value);
		}
		guiObject->blockSignals(false);
	}
	signal1<double> signal_toRebroadcast;
	std::string getName()
	{
		if (guiObject!=NULL)
			return guiObject->objectName().toStdString();
		else
			return std::string("");
	};
	void sync()
	{
		if ( type == QCHECKBOX )
		{
			signal_toRebroadcast.broadcast((double)dynamic_cast<QCheckBox*>(guiObject)->checkState());
		}
		else if ( type == QCOMBOBOX )
		{
			signal_toRebroadcast.broadcast((double)dynamic_cast<QComboBox*>(guiObject)->currentIndex());
		}
		else if ( type == QDOUBLESPINBOX )
		{
			signal_toRebroadcast.broadcast((double)dynamic_cast<QDoubleSpinBox*>(guiObject)->value());
		}
		else if ( type == QPROGRESSBAR )
		{
			signal_toRebroadcast.broadcast((double)dynamic_cast<QProgressBar*>(guiObject)->value());
		}
		else if ( type == QSPINBOX )
		{
			signal_toRebroadcast.broadcast((double)dynamic_cast<QSpinBox*>(guiObject)->value());
		}
		else if ( type == QLCDNUMBER )
		{
			signal_toRebroadcast.broadcast((double)dynamic_cast<QLCDNumber*>(guiObject)->value());
		}
	}
protected:
	QObject * guiObject;
	DoubleWidgetType type;
};
class QtRebroadcastString: public QObject, public UsesSlots
{
	Q_OBJECT
public:
	enum StringWidgetType
	{
		QLINEEDIT,
		QLABEL
	};
	QtRebroadcastString(QObject* obj = NULL)
	{
		guiObject = NULL;
		if (obj!=NULL)
			setQObject(obj);
	};
	void setQObject(QObject* obj)
	{
		if ( dynamic_cast<QLineEdit*>(obj) )
		{
			type = QLINEEDIT;
			guiObject = obj;
			//connect(dynamic_cast<QLineEdit*>(guiObject), SIGNAL(textChanged(const QString &)), this, SLOT(slot_connectToQSignal(const QString &)));
			connect(dynamic_cast<QLineEdit*>(guiObject), SIGNAL(returnPressed()),this, SLOT(slot_connectToReturnPressed()));
			connect(dynamic_cast<QLineEdit*>(guiObject), SIGNAL(editingFinished()), this, SLOT(slot_connectToEditingFinished()));
			connect(this, SIGNAL(signal_toQSlot(const QString &)), dynamic_cast<QLineEdit*>(guiObject), SLOT(setText(const QString)));
		}
		else if ( dynamic_cast<QLabel*>(obj) )
		{
			type = QLABEL;
			guiObject = obj;
			connect(this, SIGNAL(signal_toQSlot(const QString &)), dynamic_cast<QLabel*>(guiObject), SLOT(setText(const QString)));			
		}
		else 
			printf("ERROR: Attempted to add QObject: %s to a QtRebroadcastString, but this type of QObject is not registered.\n", obj->objectName().toAscii().constData());
	}
public slots:
	//void slot_connectToQSignal(const QString & str)
	//{
	//	printf("string:	%s", str.toStdString().c_str());
	//	signal_toRebroadcast.broadcast(str.toStdString());
	//}
	/*void slot_connectToQSignal()
	{
		std::string str;
		if (type == QLINEEDIT)
			str = dynamic_cast<QLineEdit*>(guiObject)->text().toStdString();
		else if (type == QLABEL)
			str = dynamic_cast<QLabel*>(guiObject)->text().toStdString();
		if(currentText.compare(str) != 0)
		{
			currentText = str;
			signal_toRebroadcast.broadcast(str);
		}
	}*/
	void slot_connectToEditingFinished()
	{
		if (type == QLINEEDIT)
		{
			std::string str;
			str = static_cast<QLineEdit*>(guiObject)->text().toStdString();
			if(currentText.compare(str) != 0)
			{
				currentText = str;
				signal_toRebroadcast.broadcast(str);
			}
		}
	}
	void slot_connectToReturnPressed()
	{		
		if (type == QLINEEDIT)
		{
			std::string str = static_cast<QLineEdit*>(guiObject)->text().toStdString();
			currentText = str;
			signal_toRebroadcast.broadcast(str);
		}
	}
signals:
	void signal_toQSlot(const QString & newString);
public:
	void slot_connectToRebroadcastSignal(std::string str)
	{
		guiObject->blockSignals(true);
		const QString & myQstr = QString(str.c_str());
		emit signal_toQSlot(myQstr);
		guiObject->blockSignals(false);
	}
	signal1<std::string> signal_toRebroadcast;
	std::string getName()
	{
		if (guiObject!=NULL)
			return guiObject->objectName().toStdString();
		else
			return std::string("");
	};
	void sync()
	{
		if ( type == QLINEEDIT )
		{
			signal_toRebroadcast.broadcast(dynamic_cast<QLineEdit*>(guiObject)->text().toStdString());
		}
		else if ( type == QLABEL )
		{
			signal_toRebroadcast.broadcast(dynamic_cast<QLabel*>(guiObject)->text().toStdString());			
		}
	}
protected:
	QObject * guiObject;
	StringWidgetType type;
	std::string currentText;
};
// This class parses QObjects and creates connections to thread-safe signals and slots
//  so that data changed in the gui can be updated in other threads automatically.  In
//  addition, changes to connected data variables in other threads can automatically 
//  change the data shown in the displayed Qt gui
class RebroadcastQtAdapter : public UsesSlots
{
public:
	RebroadcastQtAdapter(RebroadcastManager *manager = NULL, QObject * obj = NULL)
	{
		syncFlag = false;
		syncAllAdded = false;
		rebroadcastManager = manager;
		if (rebroadcastManager!=NULL)
		{
			RebroadcastVoid * sync = new RebroadcastVoid(std::string("SyncAll"));
			sync->signal_toSameThread.connect(this, &RebroadcastQtAdapter::requestSync);
			rebroadcastManager->add(sync, std::string("SyncAll"));
			rebroadcastManager->signal_broadcastComplete.connect(this, &RebroadcastQtAdapter::syncAll);
			syncAllAdded = true;
		}
		if (obj!=NULL)
			addWithChildren(obj);
	};
	void setRebroadcastManager(RebroadcastManager * manager){rebroadcastManager = manager;}
	void add(QObject * obj)
	{
		if (!syncAllAdded)
		{
			RebroadcastVoid * sync = new RebroadcastVoid(std::string("SyncAll"));
			sync->signal_toSameThread.connect(this, &RebroadcastQtAdapter::requestSync);
			rebroadcastManager->add(sync, std::string("SyncAll"));
			rebroadcastManager->signal_broadcastComplete.connect(this, &RebroadcastQtAdapter::syncAll);
			syncAllAdded = true;
		}
		if ( dynamic_cast<QSpinBox*>(obj) ||
			 dynamic_cast<QDoubleSpinBox*>(obj) ||
			 dynamic_cast<QComboBox*>(obj) ||
			 dynamic_cast<QProgressBar*>(obj) ||
			 dynamic_cast<QLCDNumber*>(obj) ||
			 dynamic_cast<QCheckBox*>(obj)	)
		{
			// Create Rebroadcasters
			QtRebroadcastDouble * qt_rb_d = new QtRebroadcastDouble(obj);
			RebroadcastDouble * rb_d = new RebroadcastDouble(obj->objectName().toStdString());
			// Make connections
			qt_rb_d->signal_toRebroadcast.connect(rb_d,  &RebroadcastDouble::slot_connectToSameThread);
			rb_d->signal_toSameThread.connect(qt_rb_d, &QtRebroadcastDouble::slot_connectToRebroadcastSignal);
			// Store connections
			qtRebroadcastDoubleList.push_back(qt_rb_d);
			rebroadcastManager->add(rb_d, qt_rb_d->getName());
		}
		else if ( (dynamic_cast<QLineEdit*>(obj) && !obj->objectName().contains(QString("qt_"))) || dynamic_cast<QLabel*>(obj) )
		{
			// Create Rebroadcasters
			QtRebroadcastString * qt_rb_s = new QtRebroadcastString(obj);
			RebroadcastString * rb_s = new RebroadcastString();
			// Make connections
			qt_rb_s->signal_toRebroadcast.connect(rb_s,  &RebroadcastString::slot_connectToSameThread);
			rb_s->signal_toSameThread.connect(qt_rb_s, &QtRebroadcastString::slot_connectToRebroadcastSignal);
			// Store connections
			qtRebroadcastStringList.push_back(qt_rb_s);
			rebroadcastManager->add(rb_s, qt_rb_s->getName());
		}
		else if ( dynamic_cast<QPushButton*>(obj) )
		{
			// Create Rebroadcasters
			QtRebroadcastVoid * qt_rb_v = new QtRebroadcastVoid(obj);
			RebroadcastVoid * rb_v = new RebroadcastVoid();
			// Make connections
			qt_rb_v->signal_toRebroadcast.connect(rb_v,  &RebroadcastVoid::slot_connectToSameThread);
			rb_v->signal_toSameThread.connect(qt_rb_v, &QtRebroadcastVoid::slot_connectToRebroadcastSignal);
			// Store connections
			qtRebroadcastVoidList.push_back(qt_rb_v);
			rebroadcastManager->add(rb_v, qt_rb_v->getName());
		}
	};
	void addWithChildren(QObject * obj)
	{
		add(obj);
		QObjectList list = getQObjectChildren(obj);
		for (int i=0; i<list.size(); i++)
		{	
			addWithChildren(list.at(i));
		}
	};
	QObjectList getQObjectChildren(QObject * obj)
	{
		QObjectList list;
		if ( dynamic_cast<QTabWidget*>(obj) )
		{
			QTabWidget * tabWidget = (QTabWidget*)obj;
			for (int i=0; i<tabWidget->count(); i++)
			{
				list.push_back(tabWidget->widget(i));
			}
		}
		else
			list = obj->children();
		return list;
	}
	void requestSync()
	{
		syncFlag = true;
	}
	void syncAll()
	{
		if (syncFlag)
		{
			std::vector<QtRebroadcastVoid*>::iterator voidIter = qtRebroadcastVoidList.begin();
			for(;voidIter!=qtRebroadcastVoidList.end();++voidIter)
			{
				(*voidIter)->sync();
			}
			std::vector<QtRebroadcastDouble*>::iterator doubleIter = qtRebroadcastDoubleList.begin();
			for(;doubleIter!=qtRebroadcastDoubleList.end();++doubleIter)
			{
				(*doubleIter)->sync();
			}
			std::vector<QtRebroadcastString*>::iterator stringIter = qtRebroadcastStringList.begin();
			for(;stringIter!=qtRebroadcastStringList.end();++stringIter)
			{
				(*stringIter)->sync();
			}
		}
		syncFlag = false;
	}
	~RebroadcastQtAdapter()
	{
		std::vector<QtRebroadcastVoid*>::iterator voidIter = qtRebroadcastVoidList.begin();
		for(;voidIter!=qtRebroadcastVoidList.end();++voidIter)
		{
			delete (*voidIter);
		}
		std::vector<QtRebroadcastDouble*>::iterator doubleIter = qtRebroadcastDoubleList.begin();
		for(;doubleIter!=qtRebroadcastDoubleList.end();++doubleIter)
		{
			delete (*doubleIter);
		}
		std::vector<QtRebroadcastString*>::iterator stringIter = qtRebroadcastStringList.begin();
		for(;stringIter!=qtRebroadcastStringList.end();++stringIter)
		{
			delete (*stringIter);
		}
	}
protected:
	std::vector<QtRebroadcastVoid*> qtRebroadcastVoidList;
	std::vector<QtRebroadcastDouble*> qtRebroadcastDoubleList;
	std::vector<QtRebroadcastString*> qtRebroadcastStringList;
	RebroadcastManager * rebroadcastManager;
	bool syncAllAdded;
	bool syncFlag;
};