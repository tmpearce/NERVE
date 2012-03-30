#include "SceneElement.h"


void SceneElement::accept(SceneElementVisitor * visitor)
{
	visitor->visit(this);
}
void SceneElement::traverse(SceneElementVisitor *visitor)
{
	for(std::list<SceneElement*>::iterator itr = childList.begin();
		itr != childList.end(); ++itr)
	{
		(*itr)->accept(visitor);
	}
}
void SceneElement::removeModule(SceneElementModule* toRemove)
{
	ModuleList::iterator iter = moduleList.begin();
	for(;iter!=moduleList.end();)
	{
		if((*iter)== toRemove)
		{
			if((*iter)->getDeleteOnRemove()==true)
			{
				delete (*iter);
				*iter = NULL;
			}
			iter = moduleList.erase(iter);				
		}
		else
		{
			++iter;
		}
	}
}
SceneElement::~SceneElement()
{
	for(std::list<SceneElementModule*>::iterator i = moduleList.begin();i!=moduleList.end();++i)
	{
		delete (*i);
	}
}