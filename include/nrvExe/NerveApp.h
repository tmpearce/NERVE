#pragma once
#ifdef BUILD_NERVEAPPLICATION_LIBRARY
#define NERVEAPPLICATION __declspec(dllexport)
#else
#define NERVEAPPLICATION __declspec(dllimport)
#endif

class NerveApplication;
class NERVEAPPLICATION NerveApp
{
public:
	NerveApp();
	~NerveApp();
	void launch();
private:
	NerveApplication* app;
};