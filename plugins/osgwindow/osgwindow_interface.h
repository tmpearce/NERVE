#pragma once
#include "nrvApp/NerveAPI.h"
class Window;
class IImpl;
namespace osg{
	class Group;
	class NodeCallback;
}

class IOSGWindow : public IPlugin
{
public:
	void setScene(osg::Group*);
	void setCameraCallback(osg::NodeCallback*);
	void setupInWindow();
	void setupFullscreen();
	void setMonitorNum(int i);
	void setMirrorLR(bool b);
	void setMaxFrameRate(float fr);
	void setUseStereoMode(bool b);
	void setCameraMode(int i);
	void applyChanges();
private:
	friend class Impl;
	IImpl* pIImpl;
	IOSGWindow(Window*w);
	IOSGWindow(const IOSGWindow& other);
};