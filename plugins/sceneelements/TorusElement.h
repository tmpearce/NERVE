#pragma once

#include "MobileElement.h"
#include <osg/Drawable>
namespace osg
{
	class Drawable;
}

class TorusElement : public MobileElement
{
public:
	TorusElement(GMutexArray *inputArray, GMutexArray *outputArray):MobileElement(inputArray, outputArray)
	{
		_axialRadius=1.0;
		_tubeRadius=1.0;
		addDrawable(createTorus(_axialRadius, _tubeRadius));
	}
	void accept(SceneElementVisitor *visitor);
	TorusElement* asTorus(){return this;}
	void setScaleTorus (float axial_radius, float tube_radius);	
	void setAxialRadius(float axial_radius);
	void setTubeRadius(float tube_radius);
	void makeScaleUniform(float scale_factor);
	float getAxialRadius(){return _axialRadius;}
	float getTubeRadius(){return _tubeRadius;}
	
	void setScale(osg::Vec3 invalid_option);
	void setScale(double invalidx, double invalidy, double invalidz);
	void setScale(double invalid);
protected:
	float _axialRadius;
	float _tubeRadius;
private:
	//osg::ref_ptr<osg::Drawable> torusDrawable;
	osg::ref_ptr<osg::Drawable> createTorus (float axial_radius, float tube_radius);
};



