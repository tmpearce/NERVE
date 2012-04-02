#pragma once
#include "nrvThread/NerveThread.h"
#include "nrvThread/NerveModule.h"
class CenterOut;
class LogicModule : public NerveModule
{
public:
	LogicModule(CenterOut* c):centerOut(c){}
protected:
	void moduleOperation(NerveModuleUser*);
private:
	CenterOut* centerOut;
};
class CenterOut
{
public:
private:

};
