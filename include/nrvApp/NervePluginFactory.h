#pragma once
#include "nrvApp\NervePluginBase.h"
//class QDialog;
class NerveAPI;


class NervePluginFactory
{
public:
	static int getNerveVersion();
	virtual NervePluginBase*	createPluginObject() = 0;
	virtual void				cleanUpPluginObject(NervePluginBase*,NerveAPI*) = 0;
	virtual const char*			getName() = 0;
	virtual void                selfCleanup(){delete this;}
	/* Read the following about selfCleanup():
		This method is included to enable plugins to be loaded/unloaded any time before
		the end of the Nerve Application's lifetime.  Since loading a plugin requires an instance (object)
		of NervePluginFactory to be given to the application, unloading the dll without cleaning up this object
		would lead to memory leaks.  A default implementation is provided here using delete this; however, it is
		NOT GUARANTEED TO STAY THAT WAY! So to be safe, please re-implement it to handle whatever procedure your
		particular plugin uses for memory allocation. For more information about the self-cleanup process, see below:

	http://www.parashift.com/c++-faq-lite/freestore-mgmt.html
	[16.15] Is it legal (and moral) for a member function to say delete this?

	As long as you're careful, it's OK for an object to commit suicide (delete this).

	Here's how I define "careful":

	You must be absolutely 100% positively sure that this object was allocated via new (not by new[], nor by placement new, nor a local object on the stack, nor a global, nor a member of another object; but by plain ordinary new).
	You must be absolutely 100% positively sure that your member function will be the last member function invoked on this object.
	You must be absolutely 100% positively sure that the rest of your member function (after the delete this line) doesn't touch any piece of this object (including calling any other member functions or touching any data members).
	You must be absolutely 100% positively sure that no one even touches the this pointer itself after the delete this line. In other words, you must not examine it, compare it with another pointer, compare it with NULL, print it, cast it, do anything with it.
	Naturally the usual caveats apply in cases where your this pointer is a pointer to a base class when you don't have a virtual destructor.
	*/
};