#pragma once
#include "MobileElement.h"
#include <osg/Vec3>

class BoxElement : public MobileElement
{
public:
	BoxElement()
	{
		addDrawable(new osg::ShapeDrawable(new osg::Box()));
		initColliderVars();
	}
	BoxElement(GMutexArray* inputArray, GMutexArray* outputArray):MobileElement(inputArray, outputArray)
	{
		addDrawable(new osg::ShapeDrawable(new osg::Box()));
		initColliderVars();
	}
	void addDrawable(osg::Drawable* drawable)
	{
		//only add drawable once (in constructor)
		if(!drawableAdded)MobileElement::addDrawable(drawable);
	}
	virtual void accept(SceneElementVisitor * visitor);
	osg::Vec3Array* getColliderUntransformedPointArray(){return planePoints_pretransform;}
	osg::Vec3Array* getColliderUntransformedNormalArray(){return planeNormals_pretransform;}
	BoxElement* asBox(){return this;}
protected:
	osg::ref_ptr<osg::Vec3Array> planePoints_pretransform;
	osg::ref_ptr<osg::Vec3Array> planeNormals_pretransform;
	
	void initColliderVars()
	{
		planePoints_pretransform = new osg::Vec3Array();
		planeNormals_pretransform = new osg::Vec3Array();
		//+x surface
		planePoints_pretransform->push_back(osg::Vec3(0.5f, 0.0f, 0.0f));
		planeNormals_pretransform->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
		//-x surface
		planePoints_pretransform->push_back(osg::Vec3(-0.5f, 0.0f, 0.0f));
		planeNormals_pretransform->push_back(osg::Vec3(-1.0f, 0.0f, 0.0f));
		//+y surface
		planePoints_pretransform->push_back(osg::Vec3(0.0f, 0.5f, 0.0f));
		planeNormals_pretransform->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
		//-y surface
		planePoints_pretransform->push_back(osg::Vec3(0.0f, -0.5f, 0.0f));
		planeNormals_pretransform->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		//+z surface
		planePoints_pretransform->push_back(osg::Vec3(0.0f, 0.0f, 0.5f));
		planeNormals_pretransform->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
		//-z surface
		planePoints_pretransform->push_back(osg::Vec3(0.0f, 0.0f, -0.5f));
		planeNormals_pretransform->push_back(osg::Vec3(0.0f, 0.0f, -1.0f));
	}
};