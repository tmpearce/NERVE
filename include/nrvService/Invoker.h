#pragma once
#include <string>
#include <vector>
#include "nrvService/GenericArgument.h"
#include "nrvService/NerveService.h"

class InvokerBase
{
public:
	InvokerBase(unsigned int numArgs, std::string commaDelimParamNames):_maxArgs(numArgs),_minArgs(numArgs)
	{
		std::stringstream ss(commaDelimParamNames);
		std::string name;
		while(std::getline(ss, name, ',')){paramNames.push_back(name);}
		while(paramNames.size()<_maxArgs) paramNames.push_back("Unknown");
	}
	virtual ~InvokerBase()
	{
		std::map<unsigned int,GenericArg*>::iterator iter = defaultParamMap.begin();
		for(;iter!=defaultParamMap.end();++iter)
		{
			delete iter->second;
		}
	}
	virtual bool invoke(const GenericArgList& args){printf("InvokerBase::invoke (nullop)\n"); return false;}
	virtual bool testArgs(const GenericArgList& args){printf("InvokerBase::testArgs (nullop)\n"); return false;}
	virtual std::string getArgType(unsigned int paramPos){printf("InvokerBase::getArgType (nullop)\n");return "invalid";}
	virtual std::string getReturnType(){printf("InvokerBase::getReturnType (nullop)\n");return "invalid";}
	virtual std::string getArgName(unsigned int paramPos)
	{
		if(paramPos>=_maxArgs) return "out of range";
		return paramNames[paramPos].c_str();
	}
	virtual std::string getDefaultArgValAsString(unsigned int paramPos)
	{
		if(defaultParamMap.count(paramPos) > 0) return defaultParamMap[paramPos]->stringifyData();
		else return "No default value";
	}
	std::string getArgNameWithDefaultValue(unsigned int paramPos)
	{
		if(paramPos < _maxArgs)
		{
			std::string str = getArgName(paramPos);
			std::string def = getDefaultArgValAsString(paramPos);
			if(def.compare("No default value"))//compare==0 when strings match exactly
			{
				str.append(" = ");
				str.append(def);
			}
			return str;
		}
		else{return "";}
	}

	template<class T>
	inline typename add_ptr<T>::type extractPtr(GenericArg* arg, unsigned int argNum)
	{
		if(arg==0)//arg is invalid; return a default value if possible
		{
			if(defaultParamMap.count(argNum) == 0) return 0;
			arg = defaultParamMap[argNum];
			return arg->getDataPtr<T>();
		}
		add_ptr<T>::type t = arg->getDataPtr<T>();
		if(t) return t;

		return 0;
	}
	template<class T>
	inline void setDefaultParameter(unsigned int paramPos, const T& default_val)
	{
		if(paramPos<_maxArgs)
		{
			if(defaultParamMap.count(paramPos) > 0) delete defaultParamMap[paramPos];
			defaultParamMap[paramPos] = new GenericArg(default_val);

			//calc min args needed based on defaults set
			_minArgs = _maxArgs;
			for(unsigned int argNum = _maxArgs; argNum-1 > 0; --argNum)
			{
				unsigned int position = argNum-1;
				if(defaultParamMap.count(position) > 0) --_minArgs;
				else break;
			}
		}
	}
	GenericArg* getDefaultGenericArg(unsigned int paramPos)
	{
		if(defaultParamMap.count(paramPos) > 0) return defaultParamMap[paramPos];
		else return 0;
	}
	int getMaxArgs() const {return _maxArgs;}
	int getMinArgs() const {return _minArgs;}

	
	static InvocableInfo infoFromInvoker(InvokerBase* invoker,
								  const std::string& funcName,
								  const std::string& funcDescription = "", 
								  const std::string& commaDelimArgDescriptions = "")
	{
		InvocableInfo info(funcName,invoker->getReturnType(),invoker->getMaxArgs(), funcDescription);
		std::vector<std::string> argDesc;
		std::stringstream ss(commaDelimArgDescriptions);
		std::string desc;
		while(std::getline(ss, desc, ',')) argDesc.push_back(desc);
		while(argDesc.size()<(unsigned int) invoker->getMaxArgs()) argDesc.push_back("");
		for(int i = 0; i<invoker->getMaxArgs();++i)
		{
			info.pushArg(invoker->getArgType(i),
						 invoker->getArgNameWithDefaultValue(i),
						 argDesc[i]);
		}
		return info;
	}
protected:
	const unsigned int _maxArgs;
	unsigned int       _minArgs;//if defaults are used, what's the minimum number of args
	std::map<unsigned int,GenericArg*> defaultParamMap;
	std::vector<std::string> paramNames;
};

/*************************************
Template types (up to 9 parameters are supported)
The template types must match the paramters of the function
the invoker is to call
/*************************************/
template<	class ClassWithMemFunc,
			class ReturnType = void,
			class ArgType1 = void,
			class ArgType2 = void,
			class ArgType3 = void,
			class ArgType4 = void,
			class ArgType5 = void,
			class ArgType6 = void,
			class ArgType7 = void,
			class ArgType8 = void,
			class ArgType9 = void >
class Invoker : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1,ArgType2,ArgType3,ArgType4,ArgType5,ArgType6,ArgType7,ArgType8,ArgType9);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(9,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		add_ptr<ArgType6>::type a6 = extractPtr<ArgType6>(args.argAt(5),5);
		add_ptr<ArgType7>::type a7 = extractPtr<ArgType7>(args.argAt(6),6);
		add_ptr<ArgType8>::type a8 = extractPtr<ArgType8>(args.argAt(7),7);
		add_ptr<ArgType9>::type a9 = extractPtr<ArgType9>(args.argAt(8),8);
		

		if(a1 && a2 && a3 && a4 && a5 && a6 && a7 && a8 && a9)
		{
			(obj->*func)(*a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		add_ptr<ArgType6>::type a6 = extractPtr<ArgType6>(args.argAt(5),5);
		add_ptr<ArgType7>::type a7 = extractPtr<ArgType7>(args.argAt(6),6);
		add_ptr<ArgType8>::type a8 = extractPtr<ArgType8>(args.argAt(7),7);
		add_ptr<ArgType9>::type a9 = extractPtr<ArgType9>(args.argAt(8),8);
		

		if(a1 && a2 && a3 && a4 && a5 && a6 && a7 && a8 && a9)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		case 1: return typeid(ArgType2).name(); break;
		case 2: return typeid(ArgType3).name(); break;
		case 3: return typeid(ArgType4).name(); break;
		case 4: return typeid(ArgType5).name(); break;
		case 5: return typeid(ArgType6).name(); break;
		case 6: return typeid(ArgType7).name(); break;
		case 7: return typeid(ArgType8).name(); break;
		case 8: return typeid(ArgType9).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType >
class Invoker<ClassWithMemFunc, ReturnType,void,void,void,void,void,void,void,void,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)();
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall):
		obj(ptrToObject),func(functionToCall),InvokerBase(0,""){}
	bool invoke(const GenericArgList& args){(obj->*func)(); return true;}
	bool testArgs(const GenericArgList& args){return true;}
	std::string getArgType(unsigned int paramPos)
	{
		return typeid(void).name();
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType,
			class ArgType1 >
class Invoker<ClassWithMemFunc, ReturnType, ArgType1,void,void,void,void,void,void,void,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(1,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		
		if(a1)
		{
			(obj->*func)(*a1);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		
		if(a1)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType,
			class ArgType1,
			class ArgType2 >
class Invoker<ClassWithMemFunc, ReturnType, ArgType1,ArgType2,void,void,void,void,void,void,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1,ArgType2);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(2,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		
		if(a1 && a2)
		{
			(obj->*func)(*a1,*a2);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		
		if(a1 && a2)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		case 1: return typeid(ArgType2).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType,
			class ArgType1,
			class ArgType2,
			class ArgType3 >
class Invoker<ClassWithMemFunc, ReturnType, ArgType1,ArgType2,ArgType3,void,void,void,void,void,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1,ArgType2,ArgType3);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(3,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		
		if(a1 && a2 && a3)
		{
			(obj->*func)(*a1,*a2,*a3);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		
		if(a1 && a2 && a3)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		case 1: return typeid(ArgType2).name(); break;
		case 2: return typeid(ArgType3).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType,
			class ArgType1,
			class ArgType2,
			class ArgType3,
			class ArgType4 >
class Invoker<ClassWithMemFunc, ReturnType, ArgType1,ArgType2,ArgType3,ArgType4,void,void,void,void,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1,ArgType2,ArgType3,ArgType4);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(4,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		
		if(a1 && a2 && a3 && a4)
		{
			(obj->*func)(*a1,*a2,*a3,*a4);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		
		if(a1 && a2 && a3 && a4)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		case 1: return typeid(ArgType2).name(); break;
		case 2: return typeid(ArgType3).name(); break;
		case 3: return typeid(ArgType4).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType,
			class ArgType1,
			class ArgType2,
			class ArgType3,
			class ArgType4,
			class ArgType5>
class Invoker<ClassWithMemFunc, ReturnType, ArgType1,ArgType2,ArgType3,ArgType4,ArgType5,void,void,void,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1,ArgType2,ArgType3,ArgType4,ArgType5);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(5,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		
		if(a1 && a2 && a3 && a4 && a5)
		{
			(obj->*func)(*a1,*a2,*a3,*a4,*a5);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		
		if(a1 && a2 && a3 && a4 && a5)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		case 1: return typeid(ArgType2).name(); break;
		case 2: return typeid(ArgType3).name(); break;
		case 3: return typeid(ArgType4).name(); break;
		case 4: return typeid(ArgType5).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType,
			class ArgType1,
			class ArgType2,
			class ArgType3,
			class ArgType4,
			class ArgType5,
			class ArgType6>
class Invoker<ClassWithMemFunc, ReturnType, ArgType1,ArgType2,ArgType3,ArgType4,ArgType5,ArgType6,void,void,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1,ArgType2,ArgType3,ArgType4,ArgType5,ArgType6);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(6,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		add_ptr<ArgType6>::type a6 = extractPtr<ArgType6>(args.argAt(5),5);
		
		if(a1 && a2 && a3 && a4 && a5 && a6)
		{
			(obj->*func)(*a1,*a2,*a3,*a4,*a5,*a6);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		add_ptr<ArgType6>::type a6 = extractPtr<ArgType6>(args.argAt(5),5);
		
		if(a1 && a2 && a3 && a4 && a5 && a6)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		case 1: return typeid(ArgType2).name(); break;
		case 2: return typeid(ArgType3).name(); break;
		case 3: return typeid(ArgType4).name(); break;
		case 4: return typeid(ArgType5).name(); break;
		case 5: return typeid(ArgType6).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType,
			class ArgType1,
			class ArgType2,
			class ArgType3,
			class ArgType4,
			class ArgType5,
			class ArgType6,
			class ArgType7 >
class Invoker<ClassWithMemFunc, ReturnType, ArgType1,ArgType2,ArgType3,ArgType4,ArgType5,ArgType6,ArgType7,void,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1,ArgType2,ArgType3,ArgType4,ArgType5,ArgType6,ArgType7);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(7,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		add_ptr<ArgType6>::type a6 = extractPtr<ArgType6>(args.argAt(5),5);
		add_ptr<ArgType7>::type a7 = extractPtr<ArgType7>(args.argAt(6),6);
		
		if(a1 && a2 && a3 && a4 && a5 && a6 && a7)
		{
			(obj->*func)(*a1,*a2,*a3,*a4,*a5,*a6,*a7);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		add_ptr<ArgType6>::type a6 = extractPtr<ArgType6>(args.argAt(5),5);
		add_ptr<ArgType7>::type a7 = extractPtr<ArgType7>(args.argAt(6),6);
		
		if(a1 && a2 && a3 && a4 && a5 && a6 && a7)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		case 1: return typeid(ArgType2).name(); break;
		case 2: return typeid(ArgType3).name(); break;
		case 3: return typeid(ArgType4).name(); break;
		case 4: return typeid(ArgType5).name(); break;
		case 5: return typeid(ArgType6).name(); break;
		case 6: return typeid(ArgType7).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
template<	class ClassWithMemFunc,
			class ReturnType,
			class ArgType1,
			class ArgType2,
			class ArgType3,
			class ArgType4,
			class ArgType5,
			class ArgType6,
			class ArgType7,
			class ArgType8>
class Invoker<ClassWithMemFunc, ReturnType, ArgType1,ArgType2,ArgType3,ArgType4,ArgType5,ArgType6,ArgType7,ArgType8,void> : public InvokerBase
{
public:
	typedef ReturnType (ClassWithMemFunc::*MemFunc)(ArgType1,ArgType2,ArgType3,ArgType4,ArgType5,ArgType6,ArgType7,ArgType8);
	Invoker(ClassWithMemFunc* ptrToObject, MemFunc functionToCall, std::string commaDelimParamNames):
		obj(ptrToObject),func(functionToCall),InvokerBase(8,commaDelimParamNames){}
	bool invoke(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		add_ptr<ArgType6>::type a6 = extractPtr<ArgType6>(args.argAt(5),5);
		add_ptr<ArgType7>::type a7 = extractPtr<ArgType7>(args.argAt(6),6);
		add_ptr<ArgType8>::type a8 = extractPtr<ArgType8>(args.argAt(7),7);

		if(a1 && a2 && a3 && a4 && a5 && a6 && a7 && a8)
		{
			(obj->*func)(*a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8);
			return true;
		}
		return false;
	}
	bool testArgs(const GenericArgList& args)
	{
		add_ptr<ArgType1>::type a1 = extractPtr<ArgType1>(args.argAt(0),0);
		add_ptr<ArgType2>::type a2 = extractPtr<ArgType2>(args.argAt(1),1);
		add_ptr<ArgType3>::type a3 = extractPtr<ArgType3>(args.argAt(2),2);
		add_ptr<ArgType4>::type a4 = extractPtr<ArgType4>(args.argAt(3),3);
		add_ptr<ArgType5>::type a5 = extractPtr<ArgType5>(args.argAt(4),4);
		add_ptr<ArgType6>::type a6 = extractPtr<ArgType6>(args.argAt(5),5);
		add_ptr<ArgType7>::type a7 = extractPtr<ArgType7>(args.argAt(6),6);
		add_ptr<ArgType8>::type a8 = extractPtr<ArgType8>(args.argAt(7),7);

		if(a1 && a2 && a3 && a4 && a5 && a6 && a7 && a8)
		{
			return true;
		}
		return false;
	}
	std::string getArgType(unsigned int paramPos)
	{
		switch(paramPos)
		{
		case 0: return typeid(ArgType1).name(); break;
		case 1: return typeid(ArgType2).name(); break;
		case 2: return typeid(ArgType3).name(); break;
		case 3: return typeid(ArgType4).name(); break;
		case 4: return typeid(ArgType5).name(); break;
		case 5: return typeid(ArgType6).name(); break;
		case 6: return typeid(ArgType7).name(); break;
		case 7: return typeid(ArgType8).name(); break;
		default: return typeid(void).name();
		}
	}
	std::string getReturnType()
	{
		return typeid(ReturnType).name();
	}
private:
	ClassWithMemFunc* obj;
	MemFunc           func;
};
