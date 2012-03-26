#pragma once
#include "osgwindow_guts.h"

class IImpl
{
public:
	IImpl(Window* w):window(w){}
	void setScene(osg::Group* n){window->setSceneData(n);}
	void setCameraCallback(osg::NodeCallback* n){window->setCameraCallback(n);}
	void setupInWindow(){params.windowed = true;}
	void setupFullscreen(){params.windowed = false;}
	void setMonitorNum(int i){params.screenNum = i;}
	void setMirrorLR(bool b){params.mirrored = b;}
	void setMaxFrameRate(float fr){params.frameRate = fr;}
	void setUseStereoMode(bool b){params.stereo = b;}
	void setCameraMode(int i){params.camManipOption = i;}
	void applyChanges(){window->configure(params);}
private:
	Window* window;
	WindowParams params;
};