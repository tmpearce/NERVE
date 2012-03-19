#include "ui_QtTutorial.h"
#include <QIntValidator>
#include <QString>
#include "test.h"

class C : public QObject
{
	Q_OBJECT
public:
	C(QObject* parent = 0):QObject(parent)
	{
	}
	~C(){printf("C dtor\n");}
	void send(){emit s();}
signals:
	void s();
protected:
	void connectNotify(const char* signal)
	{
		/*if(signal==SIGNAL(s))*/ printf("connect\n");
	}
	void disconnectNotify(const char* signal)
	{
		/*if(signal==SIGNAL(s))*/ printf("disconnect\n");
	}
};
class R : public QObject
{
	Q_OBJECT
public:
	R(C* c, QObject* parent = 0):QObject(parent),_c(c)
	{
		connect(c,SIGNAL(s()),this,SLOT(r()));
		
	}
	~R(){disconnect(_c,SIGNAL(s()),this,SLOT(r()));printf("R dtor\n");}
	C* _c;
public slots:
		void r(){printf("signal received\n");}
};

class B : public QObject {
public:
	B(QObject* parent=0):QObject(parent){}
  ~B(){}
};

class A : public B {
  
};


class TutorialGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	TutorialGui()
	{
        A* a = new A();
		bool b = false;
		QString s = QString::number(b);
		printf("number: %s\n",s.toStdString().c_str());
		ui.setupUi(this);
		QIntValidator* validator = new QIntValidator(this);
		ui.val1->setValidator(validator);
		ui.val2->setValidator(validator);
		connect(ui.val1,SIGNAL(textChanged(const QString&)),this,SLOT(val1changed(const QString&)));
		connect(ui.val2,SIGNAL(textChanged(const QString&)),this,SLOT(val2changed(const QString&)));

		C* sen = new C();
		R* rec = new R(sen);
		
		sen->send();
		delete rec;
		delete sen;
	}
public slots:
	void val1changed(const QString& s)
	{
		val1=s.toInt();
		add();
	}
	void val2changed(const QString& s)
	{
		val2=s.toInt();
		add();
	}
protected:
	void add()
	{
		ui.val3->setText(QString::number(val1+val2));
	}

	Ui::TutorialDialog ui;
	int val1;
	int val2;
};