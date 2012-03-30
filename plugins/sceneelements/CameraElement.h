#pragma once

#include "SceneElement.h"
#include "SceneElementDataContainers.h"

class CameraElement : public SceneElement
{
public:
	CameraElement();
	CameraElement(GMutexArray* outputArray);
	CameraElement* asCamera(){return this;}
	void setBackgroundColor(osg::Vec4 color){ti.diffuseColor = color;}
	void setBackgroundColor(float r, float g, float b){ti.diffuseColor.set(r, g, b, 0.0);}
	void setName(const std::string &name);
	void addChild(SceneElement* child)
	{
		grp->addChild(child->getPointerTop());
		childList.push_back(child);
	}
	void setViewMatrix(osg::Matrix viewMatrix){ti.localMatrix = viewMatrix;}
	osg::Node* getPointerTop() const {return topNode;}
	osg::Node* getPointerBottom() const {return bottomNode;}
	virtual void accept(SceneElementVisitor * visitor);
	void addMirror();
protected:
	TransformInfo ti;
	SwitchInfo si;
	std::vector<SceneElement*> childList;
	osg::ref_ptr<osg::Camera> camera;
	osg::ref_ptr<osg::Switch> sw;
	osg::ref_ptr<osg::Group> grp;
	osg::Node *bottomNode, *topNode;
	
};