#pragma once

//#include "SceneElementDataContainers.h"
#include <osg/Node>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>
#include <osg/Switch>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/ShapeDrawable>
//#include <vector>
#include <list>
#include <algorithm>

#include "SceneElementVisitor.h"
#include "SceneElementModule.h"

class GMutexArray;
class GroupElement;
class MobileElement;
class CollidableElement;
class SphereElement;
class BoxElement;
class TorusElement;
class CameraElement;

namespace TaskTools
{
	class ColliderBase;
	class SphereCollider;
}

class SceneElement
{
public:
	typedef std::list<SceneElement*>		SceneElementList;
	typedef std::list<SceneElementModule*>	ModuleList;


	virtual osg::Node* getPointerTop() const = 0;
	virtual osg::Node* getPointerBottom() const = 0;

	virtual GroupElement* asGroup(){return 0;}
	virtual CollidableElement* asCollidable(){return 0;}
	virtual MobileElement* asMobile(){return 0;}
	virtual SphereElement* asSphere(){return 0;}
	virtual BoxElement* asBox(){return 0;}
	virtual TorusElement* asTorus(){return 0;}
	virtual CameraElement* asCamera(){return 0;}
	void addModule(SceneElementModule* module){moduleList.push_back(module);}
	void removeModule(SceneElementModule* toRemove);
	ModuleList getModuleList() {return moduleList;}
	virtual void addChild(SceneElement* child){childList.push_back(child);}
	void addAsChildOfSceneElement(SceneElement* parent){parent->addChild(this);}
	virtual void accept(SceneElementVisitor * visitor);
	virtual void traverse(SceneElementVisitor * visitor);
	virtual ~SceneElement();
	
protected:
	
	SceneElementList	childList;
	ModuleList			moduleList;
};