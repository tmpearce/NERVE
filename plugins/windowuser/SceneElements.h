#pragma once
#include <osg/MatrixTransform>
#include <osg/NodeCallback>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/CullFace>
#include <osg/Vec3>
#include <osg/Quat>
#include "nrvToolbox/TriBuf.h"

struct MobileInfo
{
	osg::Matrix matrix;
	osg::Vec4 color;
	float transparency;
};
class MobileCallback : public osg::NodeCallback
{
public:
	MobileCallback(){}
	~MobileCallback(){}
	void operator()(osg::Node* n, osg::NodeCallback* nc)
	{
		osg::MatrixTransform* m = static_cast<osg::MatrixTransform*>(n);
	}
	TriBuf<MobileInfo>* getInfoBufferPtr(){return &infoBuffer;}
private:
	TriBuf<MobileInfo> infoBuffer;
};

class GroupElement
{
public:
	GroupElement()
	{
		osgNode = new osg::Group();
	}
	void addChild(osg::Node* child){osgNode->addChild(child);}
	osg::Group* getNodePtr(){return osgNode;}
private:
	osg::ref_ptr<osg::Group> osgNode;
};
class MobileElement : public GroupElement
{
public:
	MobileElement()
	{
		osgNode = new osg::MatrixTransform();
		MobileCallback* mc = new MobileCallback();//pointer managed by osg::ref_ptr
		osgNode->setUpdateCallback(mc);
		infoBuffer = mc->getInfoBufferPtr();

		geodeCullFrontFace = new osg::Geode();
		geodeCullBackFace = new osg::Geode();
		geodeCullFrontFace->getOrCreateStateSet()->setAttributeAndModes(new osg::CullFace(osg::CullFace::FRONT));
		geodeCullBackFace->getOrCreateStateSet()->setAttributeAndModes(new osg::CullFace(osg::CullFace::BACK));
		osgNode->addChild(geodeCullFrontFace);
		osgNode->addChild(geodeCullBackFace);

		scale_.set(1.,1.,1.);
		rot_ = osg::Quat(0.,osg::Vec3(0.,1.,0.));
		trans_.set(0.,0.,0.);

		osgNode->getOrCreateStateSet()->setDataVariance(osg::Object::DYNAMIC);
		osgNode->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
		osgNode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	}
	~MobileElement(){}
	void setTranslation(osg::Vec3 trans){trans_=trans;updateMatrix();updateInfo();}
	void setTranslation(double x, double y, double z){setTranslation(osg::Vec3(x,y,z));}
	void setScale(osg::Vec3 scale){scale_=scale;updateMatrix();updateInfo();}
	void setScale(double x, double y, double z){setScale(osg::Vec3(x,y,z));}
	void setRotation(osg::Quat rot){rot_=rot;updateMatrix();updateInfo();}
	void setColor(osg::Vec4 color){ myInfo.color = color;updateInfo();}
	void setColor(float r, float g, float b){ myInfo.color = osg::Vec4(r, g, b, 0.0); }
	void setTransparency(float transparency) { myInfo.transparency = transparency;updateInfo();}
	void addDrawable(osg::Drawable* drawable)
	{
		geodeCullFrontFace->addDrawable(drawable);
		geodeCullBackFace->addDrawable(drawable);
	}
private:
	osg::ref_ptr<osg::MatrixTransform> osgNode;
	osg::ref_ptr<osg::Geode> geodeCullFrontFace;
	osg::ref_ptr<osg::Geode> geodeCullBackFace;
	TriBuf<MobileInfo>* infoBuffer;
	osg::Vec3 trans_;
	osg::Vec3 scale_;
	osg::Quat rot_;
	MobileInfo myInfo;

	void updateMatrix()
	{
		myInfo.matrix = osg::Matrix::scale(scale_) *
						osg::Matrix::rotate(rot_) * 
						osg::Matrix::translate(trans_);
	}
	void updateInfo(){infoBuffer->setData(myInfo);}
};

class SphereElement : public MobileElement
{
public:
	SphereElement(){addDrawable(new osg::ShapeDrawable(new osg::Sphere()));}
};