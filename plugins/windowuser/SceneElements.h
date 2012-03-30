#pragma once
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <osg/Camera>
#include <osg/NodeCallback>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/CullFace>
#include <osg/Vec3>
#include <osg/Quat>
#include <osg/Material>
#include "nrvToolbox/TriBuf.h"

struct MobileInfo
{
	MobileInfo():transparency(0.),color(.7f, .7f, .7f, 0.f),matrix(osg::Matrix::scale(1.f,1.f,1.f)){}
	osg::Matrix matrix;
	osg::Vec4 color;
	float transparency;
	bool operator==(const MobileInfo& rhs){return matrix==rhs.matrix && color==rhs.color && transparency==rhs.transparency;}
	bool operator!=(const MobileInfo& rhs){return !(operator==(rhs));}
};
class CameraCallback : public osg::NodeCallback
{
public:
	CameraCallback(){}
	~CameraCallback(){}
	void operator()(osg::Node* n, osg::NodeVisitor* nv)
	{
		osg::Matrix temp = buffer.getData();
		//if(temp != viewMatrix)
		{
			osg::Camera* c = static_cast<osg::Camera*>(n);
			viewMatrix = temp;
			c->setViewMatrix(viewMatrix);
		}
		traverse(n,nv);
	}
	void setViewMatrix(osg::Matrix m)
	{
		buffer.setData(m);
	}
private:
	TriBuf<osg::Matrix> buffer;
	osg::Matrix viewMatrix;
};
class MobileCallback : public osg::NodeCallback
{
public:
	MobileCallback(){material = new osg::Material();}
	~MobileCallback(){}
	void operator()(osg::Node* n, osg::NodeVisitor* nv)
	{
		MobileInfo temp = infoBuffer.getData();
		if(temp != settings)
		{
			settings = temp;
			osg::MatrixTransform* m = static_cast<osg::MatrixTransform*>(n);
			m->setMatrix(settings.matrix);
			
			material->setDiffuse(osg::Material::FRONT_AND_BACK, settings.color);
			material->setTransparency(osg::Material::FRONT_AND_BACK, settings.transparency);
			if(settings.transparency==0.0) 
			{
				m->getOrCreateStateSet()->setRenderingHint(osg::StateSet::OPAQUE_BIN);
				m->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::OFF);
			}
			else
			{
				m->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);			
				m->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
			}
			m->getOrCreateStateSet()->setAttribute(material);
		
		}
		traverse(n,nv);
	}
	TriBuf<MobileInfo>* getInfoBufferPtr(){return &infoBuffer;}
private:
	TriBuf<MobileInfo> infoBuffer;
	MobileInfo settings;
	osg::ref_ptr<osg::Material> material;
};
class SwitchCallback : public osg::NodeCallback
{
public:
	SwitchCallback(){}
	~SwitchCallback(){}
	void operator()(osg::Node* n, osg::NodeVisitor* nv)
	{
		osg::Switch* s = static_cast<osg::Switch*>(n);
		if(atomic==1)s->setAllChildrenOn();
		else s->setAllChildrenOff();
		traverse(n,nv);
	}
	void enable(bool b)
	{
		b ? atomic.exchange(1) : atomic.exchange(0);
	}
private:
	OpenThreads::Atomic atomic;
};
class SceneElement : public osg::Referenced
{
public:
	SceneElement* getParent(int i=0)
	{
		if(parents.size()<(i+1)) return 0;
		return parents[i];
	}
	virtual osg::Node* getNodePtr(){return osgNode.get();}
	void addParent(SceneElement* parent){parents.push_back(parent);}
	virtual ~SceneElement(){}
protected:
	std::vector<SceneElement*> parents;
private:
	osg::ref_ptr<osg::Node> osgNode;
};
class GroupElement : public SceneElement
{
public:
	GroupElement()
	{
		osgNode = new osg::Group();
	}
	virtual void addChild(SceneElement* child)
	{
		children.push_back(child);
		child->addParent(this);
		osgNode->addChild(child->getNodePtr());		
	}
	virtual osg::Group* getNodePtr(){return osgNode.get();}
protected:
	std::vector<SceneElement*> children;
private:
	osg::ref_ptr<osg::Group> osgNode;	
};
class MobileElement : public GroupElement
{
public:
	MobileElement()
	{
		switchNode = new osg::Switch();
		switchCallback = new SwitchCallback();
		switchNode->setUpdateCallback(switchCallback);
		osgNode = new osg::MatrixTransform();
		MobileCallback* mc = new MobileCallback();//pointer managed by osg::ref_ptr
		osgNode->setUpdateCallback(mc);
		infoBuffer = mc->getInfoBufferPtr();
		switchNode->addChild(osgNode);

		geodeCullFrontFace = new osg::Geode();
		geodeCullBackFace = new osg::Geode();
		geodeCullFrontFace->getOrCreateStateSet()->setAttributeAndModes(new osg::CullFace(osg::CullFace::FRONT));
		geodeCullBackFace->getOrCreateStateSet()->setAttributeAndModes(new osg::CullFace(osg::CullFace::BACK));
		osgNode->addChild(geodeCullFrontFace);
		osgNode->addChild(geodeCullBackFace);

		scale_.set(1.,1.,1.);
		rot_ = osg::Quat(0.,osg::Vec3(0.,1.,0.));
		trans_.set(0.,0.,0.);

		switchNode->getOrCreateStateSet()->setDataVariance(osg::Object::DYNAMIC);
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
	void setColor(float r, float g, float b){ setColor(osg::Vec4(r,g,b,0.)); }
	void setTransparency(float transparency) { myInfo.transparency = transparency;updateInfo();}
	void setEnabled(bool on){switchCallback->enable(on);}
	void addDrawable(osg::Drawable* drawable)
	{
		geodeCullFrontFace->addDrawable(drawable);
		geodeCullBackFace->addDrawable(drawable);
	}
	virtual osg::Switch* getNodePtr(){return switchNode.get();}
	void addChild(SceneElement* child)
	{
		children.push_back(child);
		child->addParent(this);
		osgNode->addChild(child->getNodePtr());		
	}

	osg::Vec3 getTranslation(){return trans_;}
private:
	osg::ref_ptr<osg::Switch> switchNode;
	osg::ref_ptr<SwitchCallback> switchCallback;
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
	SphereElement():MobileElement(),r_(1){addDrawable(new osg::ShapeDrawable(new osg::Sphere())); setRadius(r_);}
	float getRadius(){return r_;}
	void setRadius(float r){r_=r; setScale(r_,r_,r_);}
private:
	float r_;
};