#pragma once

#include <string>
class NerveUI;

class NerveUIFactory
{
public:
	/*Class contract:
		To be written
	*/
	virtual NerveUI&		createUIObject() = 0;
	virtual void			cleanUpUIObject(NerveUI*) = 0;
	virtual std::string		getName() = 0;
};