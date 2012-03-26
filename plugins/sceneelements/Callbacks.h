#pragma once

#include <osg/NodeCallback>
#include "SceneElementDataContainers.h"


//forward declarations
namespace osg
{
	class Material;
}
class TripleBuffer;

class Callback : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* n, osg::NodeVisitor* nv)
	{
		update(n, nv);
		traverse(n, nv);
	}
protected:
	virtual void update(osg::Node* n, osg::NodeVisitor *nv)=0;//implement in derived class
};
class TransformCallback : public Callback
{
public:
	//Default constructor creates 'new' TripleBuffers - These must be 'deleted' ELSEWHERE.
	//Get pointers to these TripleBuffers by using getInBufPtr() and getOutBufPtr().
	TransformCallback();
	TransformCallback(TripleBuffer *fromTask, TripleBuffer* toTask);
		
	TripleBuffer* getInBufPtr(){return _inBuf;}
	TripleBuffer* getOutBufPtr(){return _outBuf;}
protected:
	bool changed();
	void update(osg::Node* n, osg::NodeVisitor *nv);

	TransformInfo transformInfo, prev;
	WorldInfo worldInfo;
	TripleBuffer *_inBuf, *_outBuf;
	osg::Material* mat;
};

class CameraCallback : public Callback
{
public:
	//Default constructor creates 'new' TripleBuffers - These must be 'deleted' ELSEWHERE.
	//Get pointers to these TripleBuffers by using getInBufPtr() and getOutBufPtr().
	CameraCallback();
	CameraCallback(TripleBuffer *fromTask);
	CameraCallback(const CameraCallback& rhs, const osg::CopyOp& copyop):_inBuf(rhs._inBuf){}
	/*virtual osg::Object* clone(const osg::CopyOp& copyOp)
	{
		printf("cloning cameracallback");
		return new CameraCallback(*this);
	}*/
	META_Object( ,CameraCallback);
	//const char* className() const {return "CameraCallback";}
	TripleBuffer* getInBufPtr(){return _inBuf;}
protected:
	bool changed();
	void update(osg::Node* n, osg::NodeVisitor *nv);
	TransformInfo transformInfo, prev;
	TripleBuffer *_inBuf;
};
class SwitchCallback : public Callback
{
public:
	//Default constructor creates 'new' TripleBuffers - These must be 'deleted' ELSEWHERE.
	//Get pointers to these TripleBuffers by using getInBufPtr() and getOutBufPtr().
	SwitchCallback();
	SwitchCallback(TripleBuffer *fromTask);
	
	TripleBuffer* getInBufPtr(){return _inBuf;}

protected:
	void update(osg::Node* n, osg::NodeVisitor *nv);
	SwitchInfo switchInfo;
	TripleBuffer *_inBuf;
};

class DrawableReplacer : public osg::NodeCallback
{
public:
	DrawableReplacer(osg::Drawable* newDrawable):_drawable(newDrawable){}
	virtual void operator()(osg::Node* n, osg::NodeVisitor* nv);
protected:
	osg::ref_ptr<osg::Drawable> _drawable;
	double _name;
};