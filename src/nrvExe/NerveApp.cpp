#include "nrvExe/NerveApp.h"
#include "nrvApp/NerveApplication.h"

NerveApp::NerveApp(){app=new NerveApplication;}
NerveApp::~NerveApp(){delete app;}
void NerveApp::launch(){app->launch();}