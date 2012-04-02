#include "CenterOut.h"

void LogicModule::moduleOperation(NerveModuleUser*)
{
	centerOut->performLogic();
}
void TaskStartModule::moduleOperation(NerveModuleUser*)
{
	centerOut->changeState(CenterOut::INTERTRIAL);
}