#pragma once

#include "SceneElement.h"
#include "SceneElementDataContainers.h"
#include "Callbacks.h"
#include <vector>
#include <osg/ref_ptr>
#include <osg/Vec3>
#include <osg/Quat>
#include <osg/Timer>


//forward declarations
namespace osg
{
	class Switch;
	class Geode;
	class MatrixTransform;
	class Node;
}


class MobileElement : public SceneElement
{
public:	
	MobileElement();
	MobileElement(GMutexArray* inputArray, GMutexArray* outputArray);

	void setPosition(osg::Vec3 position)
	{
		_trans=position;
		ti.localMatrix.setTrans(position); //if anything appears broken with moving objects replace this
	}
	void setPosition(double x, double y, double z){setPosition(osg::Vec3(x, y, z));}
	void setRotation(osg::Quat rotation)
	{
		_rot = rotation;
		calcMatrix();
	}
	virtual void setScale(osg::Vec3 scale)
	{
		_scale = scale;
		calcMatrix();
	}
	virtual void setScale(double x, double y, double z){setScale(osg::Vec3(x, y, z));}
	virtual void setScale(double s){setScale(osg::Vec3(s, s, s));}

	void translate(osg::Vec3 trans_vector){_trans = _trans+trans_vector; setPosition(_trans);}
	void translate(double x, double y, double z){translate(osg::Vec3(x,y,z));}
	void rotate(osg::Quat rotate_quat){_rot = rotate_quat*_rot; calcMatrix();}
	osg::Vec3 getPosition() const { return _trans; }
	osg::Quat getRotation() const { return _rot; }
	osg::Vec3 getScale() const { return _scale; }
	osg::Matrix& matrix() { return ti.localMatrix; }
	osg::Matrix matrix() const{ return ti.localMatrix; }
	void setColor(osg::Vec4 color){ ti.diffuseColor = color; }
	void setColor(float r, float g, float b){ ti.diffuseColor = osg::Vec4(r, g, b, 0.0); }
	osg::Vec4 getColor() const { return ti.diffuseColor; }
	void setTransparency(double transparency) { ti.transparency = transparency; }
	double getTransparency() const { return ti.transparency; }
	void setMaterialMode(TransformInfo::MaterialMode mode){ti.materialMode = mode;}
	osg::Vec4& color() { return ti.diffuseColor; }
	osg::Vec4 color() const { return ti.diffuseColor; }
	osg::Matrix getWorldTransformMatrix() const { return wi.worldMatrix; }
	osg::Vec3 getWorldPosition() const {return wi.position;}
	osg::Vec3 getWorldScale() const {return wi.scale;}
	osg::Quat getWorldRotation() const {return wi.orientation;}
	bool getWorldDisplayStatus() const { return wi.isOn; }
	osg::Vec3 getBoundingSphereCenter() const {return wi.boundCenter;}
	double getBoundingSphereRadius() const {return wi.boundRadius;}
	void turnOn() { si.display = true; }
	void turnOff() { si.display = false; }
	void show(bool val) { if(val) turnOn(); else turnOff(); }
	void addChild(SceneElement* child)
	{
		mt->addChild(child->getPointerTop());
		childList.push_back(child);
	}
	osg::Node* getPointerTop() const {return topNode;}
	osg::Node* getPointerBottom() const {return bottomNode;}
	virtual void addDrawable(osg::Drawable* drawable);	
	virtual void accept(SceneElementVisitor * visitor);
	//virtual bool collide(CollisionVisitorBase* collider);
	MobileElement* asMobile(){return this;}

	//bool replaceDrawable has a minimum time between updates
	//this cooldown time prevents stacking of callbacks in the
	//scenegraph and should reduce/eliminate crashes.
	//return value is true if the drawable was replaced, and false
	//if the cooldown was in effect and the drawable was not replaced.
	//If this is the case, the most recently requested geometry is
	//held in tempDrawable.
	bool replaceDrawable(osg::Drawable* drawable);
	void setDrawableReplacementCooldownLength(double duration){drawableReplacementCooldownLength=duration;}
	void setDrawableReplacementCooldownMessageStatus(bool printMessage){drawableReplacementCooldownMessageStatus=printMessage;}

	
protected:
	TransformInfo ti;
	SwitchInfo si;
	WorldInfo wi;
	osg::Vec3 _trans, _scale;
	osg::Quat _rot;
	//std::vector<SceneElement*> childList;
	osg::ref_ptr<osg::Switch> sw;
	osg::ref_ptr<osg::MatrixTransform> mt;
	osg::ref_ptr<TransformCallback> transformCallback;
	osg::ref_ptr<SwitchCallback> switchCallback;
	osg::ref_ptr<osg::Geode> geode;
	osg::ref_ptr<osg::Geode> geode2;
	
	osg::Node *topNode, *bottomNode;
	bool drawableAdded;
	
	//for dealing with replacing drawables
	osg::Timer drawableCooldownTimer;
	osg::ref_ptr<osg::Drawable> tempDrawable;
	double drawableReplacementCooldownLength;
	bool drawableReplacementOnCooldown;
	bool drawableReplacementCooldownMessageStatus;

	void calcMatrix()
	{
		ti.localMatrix = 
			osg::Matrix::scale(_scale) *
			osg::Matrix::rotate(_rot) *
			osg::Matrix::translate(_trans);
	}
private:
	void initialization();
};