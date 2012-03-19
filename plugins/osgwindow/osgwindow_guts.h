#pragma once
#include <osgViewer/Viewer>
#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Atomic>
#include <osg/ref_ptr>
#include <osg/Timer>
#include <osg/Node>
#include <osg/NodeCallback>

class ViewerThread : public OpenThreads::Thread{
public:
	ViewerThread():atomicCancel(0),frameRate(0){}
	osgViewer::Viewer* getViewer(){return &viewer;}
	void run()
	{
		viewer.realize();
		timer.setStartTick();
		while(atomicCancel==0)
		{
			if(frameRate==0)
			{
				viewer.frame();
			}
			else if( timer.time_s()>(1/frameRate) )
			{
				timer.setStartTick();
				viewer.frame();
			}

		}
	}
	void stopRendering()
	{
		--atomicCancel;
		while(this->isRunning())
		{
			this->microSleep(1000);
		}
	}
private:
	osgViewer::Viewer viewer;
	OpenThreads::Atomic atomicCancel;
	osg::Timer timer;
	double frameRate;
};

class Window
{
public:
	Window():isRendering(false),isSetup(false)
	{
	
	}
	bool setSceneData(osg::Node* root)
	{
		if(isRendering) return false;
		vt.getViewer()->setSceneData(root);
		return true;
	}
	bool setCameraCallback(osg::NodeCallback* c)
	{
		if(isRendering) return false;
		vt.getViewer()->getCamera()->setUpdateCallback(c);
		return true;
	}
	bool startRendering()
	{
		if(isRendering || !isSetup) return false;
		vt.start();
		isRendering=true;
		return true;
	}
	bool stopRendering()
	{
		if(!isRendering) return false;
		vt.stopRendering();
		return true;
	}
	bool setupWindowed()
	{
		vt.getViewer()->setUpViewInWindow(200,200,600,400,0);
		isSetup=true;
		startRendering();
		return true;
	}
	bool setupFullscreen()
	{	
		vt.getViewer()->setUpViewOnSingleScreen(0);
		isSetup=true;
		startRendering();
		return true;
	}
private:
	ViewerThread vt;
	bool isRendering;
	bool isSetup;
};