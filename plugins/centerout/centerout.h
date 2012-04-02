#pragma once
#include "nrvThread/NerveThread.h"
#include "nrvThread/NerveModule.h"
#include "nrvToolbox/TriBuf.h"
#include "SceneElements.h"

class CenterOut;
class LogicModule : public NerveModule
{
public:
	LogicModule(CenterOut* c):centerOut(c){}
protected:
	void moduleOperation(NerveModuleUser*);
private:
	CenterOut* centerOut;
};
class TaskStartModule : public NerveModule
{
public:
	TaskStartModule(CenterOut* c):centerOut(c)
	{
		setOperateAction(NerveModule::REMOVE_MODULE_AFTER_OPERATING);
		setRemoveAction(NerveModule::DELETE_MODULE);
	}
protected:
	void moduleOperation(NerveModuleUser*);
private:
	CenterOut* centerOut;
};

class CenterOut
{
public:
	struct CursorPosition
	{
		float x, y, z;
	};
	void setCursorPosition(CenterOut::CursorPosition pos){cpBuffer.setData(pos);}
private:
	TriBuf<CenterOut::CursorPosition> cpBuffer;
public:
	CenterOut()
	{
		scene = new GroupElement();
		center = new SphereElement();
		target = new SphereElement();
		cursor = new SphereElement();
		camera = new CameraCallback();

		scene->addChild(center);
		scene->addChild(target);
		scene->addChild(cursor);

		center->setTranslation(0.,0.,0.);
		target->setTranslation(10.,0.,0.);
		cursor->setTranslation(0.,0.,5.);
		center->setEnabled(true);
		cursor->setEnabled(true);
		target->setEnabled(true);
		center->setColor(1.,.5,0.);
		cursor->setColor(1.,1.,0.);
		target->setColor(0.,1.,.5);
		target->setTransparency(.3f);

		camera->setViewMatrix(osg::Matrix::lookAt(osg::Vec3(0.,-40.,0.),osg::Vec3(0.,0.,0.),osg::Vec3(0.,0.,1.)));

		changeState(CenterOut::PRE_HOLD_A);

		logicModule = new LogicModule(this);
		logicModule->setOperateAction(NerveModule::DONT_REMOVE_MODULE);
		logicModule->setRemoveAction(NerveModule::DELETE_MODULE);
		logicThread.addModule(*logicModule);
	}
	~CenterOut(){logicThread.cancel(); while(logicThread.isRunning()) OpenThreads::Thread::microSleep(1);}
	osg::ref_ptr<osg::Group> getScene(){return scene->getNodePtr();}
	osg::ref_ptr<osg::NodeCallback> getCameraCallback(){return camera.get();}
	void start()
	{
		logicThread.addModule(*(new TaskStartModule(this)));
		if(!logicThread.isRunning()) logicThread.start();
	}
private:
	osg::ref_ptr<GroupElement> scene;
	osg::ref_ptr<SphereElement> center;
	osg::ref_ptr<SphereElement> target;
	osg::ref_ptr<SphereElement> cursor;
	osg::ref_ptr<CameraCallback> camera;

	NerveThread logicThread;
	LogicModule* logicModule;
	
	enum STATE
	{
		PRE_HOLD_A,
		HOLD_A,
		MOVEMENT,
		HOLD_B,
		FEEDBACK,
		INTERTRIAL
	};
	struct State : public osg::Referenced
	{
		State(CenterOut* co):co(co){}
		virtual void begin()=0;
		virtual void check()=0;
		CenterOut* co;
	};
	osg::ref_ptr<CenterOut::State> currentState;
	struct PreHoldA : public State
	{
		osg::Timer timer;
		PreHoldA(CenterOut* c):State(c){}
		void begin()
		{
			timer.setStartTick();
			co->center->setEnabled(true);
			co->target->setEnabled(false);
			co->cursor->setEnabled(true);
			co->center->setColor(1.,.5,0.);
		}
		void check()
		{
			if(timer.time_m() > 15000) co->changeState(CenterOut::INTERTRIAL);
			if(co->spheresAreTouching(co->cursor,co->center)) co->changeState(CenterOut::HOLD_A);
		}
	};
	struct HoldA : public State
	{
		osg::Timer timer;
		HoldA(CenterOut* c):State(c){}
		void begin()
		{
			timer.setStartTick();
			co->center->setEnabled(true);
			co->target->setEnabled(false);
			co->cursor->setEnabled(true);
			co->center->setColor(1.,0.,0.);
		}
		void check()
		{
			if(timer.time_m() > 1000) co->changeState(CenterOut::MOVEMENT);
			if(co->spheresAreTouching(co->cursor,co->center) == false) co->changeState(CenterOut::INTERTRIAL);
		}
	};
	struct Movement : public State
	{
		osg::Timer timer;
		Movement(CenterOut* c):State(c){}
		void begin()
		{
			timer.setStartTick();
			co->center->setEnabled(false);
			co->target->setEnabled(true);
			co->cursor->setEnabled(true);
			co->target->setColor(0.,1.,.5);
			co->target->setTransparency(0.3f);
		}
		void check()
		{
			if(timer.time_m() > 5000) co->changeState(CenterOut::INTERTRIAL);
			if(co->spheresAreTouching(co->cursor,co->target) == true) co->changeState(CenterOut::HOLD_B);
		}
	};
	struct HoldB : public State
	{
		osg::Timer timer;
		HoldB(CenterOut* c):State(c){}
		void begin()
		{
			timer.setStartTick();
			co->center->setEnabled(false);
			co->target->setEnabled(true);
			co->cursor->setEnabled(true);
			
		}
		void check()
		{
			if(timer.time_m() > 500) co->changeState(CenterOut::FEEDBACK);
			if(co->spheresAreTouching(co->cursor,co->target) == false) co->changeState(CenterOut::INTERTRIAL);
		}
	};
	struct Feedback : public State
	{
		osg::Timer timer;
		Feedback(CenterOut* c):State(c){}
		void begin()
		{
			timer.setStartTick();
			co->center->setEnabled(false);
			co->target->setEnabled(true);
			co->cursor->setEnabled(false);
			co->target->setColor(0.,1.,0.);
			co->target->setTransparency(0);
		}
		void check()
		{
			if(timer.time_m() > 500) co->changeState(CenterOut::INTERTRIAL);	
		}
	};
	struct Intertrial : public State
	{
		osg::Timer timer;
		Intertrial(CenterOut* c):State(c){}
		void begin()
		{
			timer.setStartTick();
			co->center->setEnabled(false);
			co->target->setEnabled(false);
			co->cursor->setEnabled(false);
		}
		void check()
		{
			if(timer.time_m() > 1500) co->changeState(CenterOut::PRE_HOLD_A);	
		}
	};
	friend class LogicModule;
	friend class TaskStartModule;
	void performLogic()
	{
		updateCursor();
		currentState->check();
	}
	void changeState(CenterOut::STATE newState)
	{
		switch(newState)
		{
		case PRE_HOLD_A:	currentState = new CenterOut::PreHoldA(this); break;
		case HOLD_A:		currentState = new CenterOut::HoldA(this); break;
		case MOVEMENT:		currentState = new CenterOut::Movement(this); break;
		case HOLD_B:		currentState = new CenterOut::HoldB(this); break;
		case FEEDBACK:		currentState = new CenterOut::Feedback(this); break;
		case INTERTRIAL:	currentState = new CenterOut::Intertrial(this); break;
		}
		currentState->begin();
	}
	bool spheresAreTouching(SphereElement* s1, SphereElement* s2)
	{
		double dist2 = (s1->getTranslation() - s2->getTranslation()).length2();
		float r = s1->getRadius() + s2->getRadius();
		double r2 = r*r;
		return dist2 < r2;
	}
	void updateCursor()
	{
		CenterOut::CursorPosition p(cpBuffer.getData());
		cursor->setTranslation(p.x,p.y,p.z);
	}
};