#pragma once

#include "MobileElement.h"


class SphereElement : public MobileElement
{
public:
	SphereElement()
	{
		addDrawable(new osg::ShapeDrawable(new osg::Sphere()));
	}
	SphereElement(GMutexArray* inputArray, GMutexArray* outputArray):MobileElement(inputArray, outputArray)
	{
		addDrawable(new osg::ShapeDrawable(new osg::Sphere()));
	}
	void addDrawable(osg::Drawable* drawable)
	{
		//only add drawable once (in constructor)
		if(!drawableAdded)MobileElement::addDrawable(drawable);
	}
	using MobileElement::setScale;
	void setScale(osg::Vec3 scale)
	{
		//only scale uniformly
		if(scale.x()==scale.y() && scale.x()==scale.z()) MobileElement::setScale(scale);
	}
	void setScale(double x, double y, double z)
	{
		//only scale uniformly
		if(x==y && x==z)
		{
			MobileElement::setScale(x, y, z);
		}
	}
	void setRadius(double r){setScale(r);}
	double getRadius(){return _scale.x();}
	virtual void accept(SceneElementVisitor * visitor);
	SphereElement* asSphere(){return this;}
};