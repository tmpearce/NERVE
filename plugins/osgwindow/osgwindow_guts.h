#pragma once
#include "nrvThread/NerveThread.h"
#include "nrvThread/NerveModule.h"
#include "nrvToolbox/Pause.h"
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>

#include <osgViewer/Viewer>
#include <osg/ref_ptr>
#include <osg/Timer>
#include <osg/Group>
#include <osg/NodeCallback>
#include <osg/ref_ptr>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Vec3>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/DisplaySettings>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/GUIEventHandler>
#include <osgDB/ReadFile>
//#include "C:/osgBullet/include/osgbInteraction/HandNode.h"
#include <string>
#include "nrvToolbox/TriBuf.h"

typedef OpenThreads::ScopedLock<OpenThreads::Mutex> Lock;

class CustomEventHandler : public osgGA::GUIEventHandler{
public:
	typedef std::pair<float,float> Position;
	Position getMouseXY(){return buffer.getData();}
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		buffer.setData(Position(ea.getXnormalized(),ea.getYnormalized()));
		return false;
	}
private:
	TriBuf<Position> buffer;
};

struct WindowParams{
	bool windowed;
	bool mirrored;
	bool stereo;
	int screenNum;
	float frameRate;
	int camManipOption;
};

class ViewerModule : public NerveModule{
public:
	ViewerModule():isRealized(false),frameRate(0),sceneData(0),cameraCallback(0),customCameraManipulator(0)
	{
		timer.setStartTick();
		nomirror = new osg::Group();
		mirror = new osg::MatrixTransform();
		viewer.setSceneData(nomirror);
		displaySettings = new osg::DisplaySettings();
		viewer.getCamera()->setDisplaySettings(displaySettings);

		mirror->setMatrix(osg::Matrix::scale(-1.,1.,1.));
		
		sceneData = new osg::Group();
		mirror->addChild(sceneData);
		nomirror->addChild(sceneData);

		/*osgbInteraction::HandNode* h = new osgbInteraction::HandNode(0);
		sceneData->addChild(h);
		
		osgbInteraction::HandNode::AllParams params;
		h->getAll(params);
		params._spread01 = 0;
		params._spread12 = 3.14159/10;
		params._spread23 = 3.14159/20;
		params._spread34 = 3.14159/8;
		params._finger2.set(3.14159/3,3.14159/3);
		params._finger3.set(3.14159/3,3.14159/3);
		h->setAll(params);*/
		
		viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3(0.,-10.,3.),osg::Vec3(0.,0.,0.),osg::Vec3(0.,0.,1.));

		
	}
	~ViewerModule(){printf("ViewerModule dtor\n");}
	void moduleOperation(NerveModuleUser*)
	{
		if(!viewer.isRealized()){ OpenThreads::Thread::microSleep(1); return;}
		if(frameRate>0 && timer.time_s()<(1.0/frameRate) ){ OpenThreads::Thread::microSleep(1); return;}
		viewer.frame();
		if(frameRate>0) timer.setStartTick();
	}
private:
	osgViewer::Viewer viewer;
	osg::ref_ptr<CustomEventHandler> windowEvents;
	osg::ref_ptr<osg::Group> sceneData;
	osg::ref_ptr<osg::DisplaySettings> displaySettings;
	osg::ref_ptr<osg::NodeCallback> cameraCallback;
	osg::ref_ptr<osgGA::MatrixManipulator> customCameraManipulator;
	osg::ref_ptr<osg::MatrixTransform> mirror;
	osg::ref_ptr<osg::Group> nomirror;
	osg::Timer timer;
	bool isRealized;
	float frameRate;
	friend class ViewerParamModule;
	friend class ViewerSceneModule;
	friend class ViewerCamCallbackModule;
	friend class EventHandlerModule;

	void setSceneData(osg::Group* sd)
	{
		sceneData = sd;
		mirror->removeChild(0,1);
		nomirror->removeChild(0,1);
		mirror->addChild(sd);
		nomirror->addChild(sd);

	}
	void setCameraCallback(osg::NodeCallback* c)
	{
		cameraCallback = c;
		if(viewer.getCameraManipulator() == 0)
		{
			viewer.getCamera()->setUpdateCallback(cameraCallback);
		}
	}

	CustomEventHandler* initEventHandling()
	{
		windowEvents = new CustomEventHandler();
		viewer.addEventHandler(windowEvents);
		return windowEvents.get();
	}
};

class ViewerParamModule : public NerveModule
{
public:
	void moduleOperation(NerveModuleUser*)
	{
		if(bframeRate)vmod->frameRate = frameRate;
		if(bmirror)
		{
			if(mirror)
			{
				vmod->viewer.setSceneData(vmod->mirror);
			}
			else
			{
				vmod->viewer.setSceneData(vmod->nomirror);
			}
		}
		if(bstereo) 
		{
			vmod->viewer.getCamera()->getDisplaySettings()->setStereo(stereo);
			if(stereo)
			{
				vmod->viewer.getCamera()->getDisplaySettings()->setStereoMode(osg::DisplaySettings::HORIZONTAL_SPLIT);
			}
		}
		if(bsetup)
		{
			if(setupinwindow) vmod->viewer.setUpViewInWindow(400,100,600,400,screenNum);
			else              vmod->viewer.setUpViewOnSingleScreen(screenNum);
			vmod->viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
			vmod->viewer.realize();
		}
		if(bmanip)
		{
			switch(manip)
			{
			case 0: vmod->viewer.setCameraManipulator(0); vmod->viewer.getCamera()->setUpdateCallback(vmod->cameraCallback); break;
			case 1: vmod->viewer.getCamera()->setUpdateCallback(0); vmod->viewer.setCameraManipulator(vmod->customCameraManipulator); break;
			case 2: vmod->viewer.getCamera()->setUpdateCallback(0); vmod->viewer.setCameraManipulator(new osgGA::TrackballManipulator()); break;
			case 3: vmod->viewer.getCamera()->setUpdateCallback(0); vmod->viewer.setCameraManipulator(new osgGA::FlightManipulator()); break;
			}
		}
		pause->unpause();
	}
private:
	ViewerParamModule(ViewerModule& v, Pause& p):vmod(&v),pause(&p),bframeRate(false),bmirror(false),
									bstereo(false),bsetup(false),bmanip(false){}
	ViewerModule* vmod;
	Pause* pause;
	friend class ViewerUpdater;
	void setFrameRate(float f){frameRate=f;bframeRate=true;}
	void setMirrored(bool b){mirror=b;bmirror=true;}
	void setStereo(bool b){stereo=b;bstereo=true;}
	void setWindowed(bool b, int screennum){setupinwindow=b; screenNum=screennum; bsetup=true;}
	void setManipulator(int i){manip=i;bmanip=true;}

	float frameRate;
	bool bframeRate;
	bool mirror;
	bool bmirror;
	bool stereo;
	bool bstereo;
	bool setupinwindow;
	bool bsetup;
	int screenNum;
	int manip;
	bool bmanip;
};
class ViewerSceneModule : public NerveModule
{
public:
	void moduleOperation(NerveModuleUser*)
	{
		vmod->setSceneData(s);
		pause->unpause();
	}
private:
	ViewerSceneModule(osg::Group* scene, ViewerModule& v, Pause& p):s(scene), vmod(&v),pause(&p){}
	ViewerModule* vmod;
	Pause* pause;
	osg::ref_ptr<osg::Group> s;
	friend class ViewerUpdater;

};
class ViewerCamCallbackModule : public NerveModule
{
public:
	void moduleOperation(NerveModuleUser*)
	{
		vmod->setCameraCallback(c);
		pause->unpause();
	}
private:
	ViewerCamCallbackModule(osg::NodeCallback* cb, ViewerModule& v, Pause& p):c(cb), vmod(&v),pause(&p){}
	ViewerModule* vmod;
	Pause* pause;
	osg::ref_ptr<osg::NodeCallback> c;
	friend class ViewerUpdater;

};
class EventHandlerModule : public NerveModule
{
public:
	void moduleOperation(NerveModuleUser*)
	{
		*ptr_to_set = vmod->initEventHandling();
		pause->unpause();
	}
private:
	EventHandlerModule(ViewerModule& v, Pause& p, CustomEventHandler** ptr):vmod(&v),pause(&p),ptr_to_set(ptr){}
	ViewerModule* vmod;
	Pause* pause;
	CustomEventHandler** ptr_to_set;
	friend class ViewerUpdater;

};
class ViewerUpdater
{
public:
	ViewerUpdater(ViewerModule& v):vmod(v,pause)
	{
		vmod.setRemoveAction(NerveModule::DONT_DELETE_MODULE);
		vmod.setOperateAction(NerveModule::REMOVE_MODULE_AFTER_OPERATING);
	}
	ViewerUpdater& setFrameRate(float fr){vmod.setFrameRate(fr);return *this;}
	ViewerUpdater& setMirrored(bool b){vmod.setMirrored(b);return *this;}
	ViewerUpdater& setStereo(bool b){vmod.setStereo(b);return *this;}
	ViewerUpdater& setWindowed(bool b,int s){vmod.setWindowed(b,s);return *this;}
	ViewerUpdater& setCamManipulation(int i){vmod.setManipulator(i);return *this;}
	static void setSceneRoot(osg::Group* scene, ViewerModule& v, NerveThread& t)
	{
		Pause p;
		ViewerSceneModule m(scene, v, p);
		m.setOperateAction(NerveModule::REMOVE_MODULE_AFTER_OPERATING);
		m.setRemoveAction(NerveModule::DONT_DELETE_MODULE);
		t.addModule(m);
		p.pause();
	}
	static void setCameraCallback(osg::NodeCallback* cb, ViewerModule& v, NerveThread& t)
	{
		Pause p;
		ViewerCamCallbackModule m(cb, v, p);
		m.setOperateAction(NerveModule::REMOVE_MODULE_AFTER_OPERATING);
		m.setRemoveAction(NerveModule::DONT_DELETE_MODULE);
		t.addModule(m);
		p.pause();
	}

	static void initEventHandling(ViewerModule& v, NerveThread& t, CustomEventHandler** eh)
	{
		Pause p;
		EventHandlerModule m(v,p,eh);
		m.setOperateAction(NerveModule::REMOVE_MODULE_AFTER_OPERATING);
		m.setRemoveAction(NerveModule::DONT_DELETE_MODULE);
		t.addModule(m);
		p.pause();
	}
	void updateParams(NerveThread& viewerThread)
	{
		viewerThread.addModule(vmod);
		pause.pause();
	}
private:
	Pause pause;
	ViewerParamModule vmod;
};
class Window
{
public:
	Window():sceneRoot(0),camCallback(0),windowEvents(0)
	{
		vm = new ViewerModule();
		vm->setOperateAction(NerveModule::DONT_REMOVE_MODULE);
		vm->setRemoveAction(NerveModule::DELETE_MODULE);
		vt.addModule(*vm);
		vt.start();
	}
	~Window(){}
	bool setSceneData(osg::Group* root)
	{
		Lock the(mutex);
		sceneRoot = root;
		ViewerUpdater::setSceneRoot(sceneRoot, *vm, vt);
		return true;
	}
	bool setCameraCallback(osg::NodeCallback* c)
	{
		Lock the(mutex);
		camCallback = c;
		ViewerUpdater::setCameraCallback(c, *vm, vt);
		return true;
	}
	void loadFile(std::string file)
	{
		Lock the(mutex);
		osg::Node* n= osgDB::readNodeFile(file);
		osg::Group* g = dynamic_cast<osg::Group*>(n);
		printf("loaded file? %i\n",g!=0);
		if(g) setSceneData(g);
	}
	void configure(WindowParams wp)
	{
		Lock the(mutex);
		ViewerUpdater update(*vm);
		if(wp.windowed != params.windowed || wp.screenNum != params.screenNum) update.setWindowed(wp.windowed,wp.screenNum);
		if(wp.mirrored != params.mirrored) update.setMirrored(wp.mirrored);
		if(wp.frameRate != params.frameRate) update.setFrameRate(wp.frameRate);
		if(wp.stereo != params.stereo) update.setStereo(wp.stereo);
		if(wp.camManipOption != params.camManipOption) update.setCamManipulation(wp.camManipOption);

		update.updateParams(vt);
		params = wp;
	}
	void initWindowEvents(){ViewerUpdater::initEventHandling(*vm,vt,&windowEvents);}
	std::pair<float,float> getMousePosition()
	{
		if(windowEvents) return windowEvents->getMouseXY();
		else return std::pair<float,float>(0.,0.);
	}
private:
	NerveThread vt;
	ViewerModule * vm; 
	CustomEventHandler* windowEvents;
	WindowParams params;
	osg::ref_ptr<osg::Group> sceneRoot;
	osg::ref_ptr<osg::NodeCallback> camCallback;
	OpenThreads::Mutex mutex;
};