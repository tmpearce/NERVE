#pragma once

#include "SceneElement.h"
#include <osg/ref_ptr>

//forward declarations
namespace osg
{
	class Node;
	class Group;
}

class GroupElement : public SceneElement
{
public:	
	GroupElement(){grp = new osg::Group();}
	virtual osg::Node* getPointerTop() const {return grp;}
	virtual osg::Node* getPointerBottom() const {return grp;}
	void addChild(SceneElement* child)
	{
		grp->addChild(child->getPointerTop());
		childList.push_back(child);
	}
	virtual void accept(SceneElementVisitor * visitor);
	GroupElement* asGroup(){return this;}
protected:	
	osg::ref_ptr<osg::Group> grp;

};