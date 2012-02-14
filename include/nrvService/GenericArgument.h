#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "nrv/type_traits.h"

class GenericArgBase
{
public:
	virtual ~GenericArgBase(){}
	virtual const char* getTypeid() = 0;
	virtual GenericArgBase* clone() = 0;
	virtual std::string stringifyData() = 0;
	template <class T>
	inline bool test();
	template <class T>
	inline bool getDataByRef(T& data);
	//NOTE: compiler error c2770 (msvc2008) will result from using a reference type as template parameter
	//eg getData<int&>() will fail to compile.  This is a good thing, as GenericArgT<T&> don't exist anyway.
	//User pointers instead (GenericArgT<int*> rather than GenericArgT<int&>).
	template <class T>
	inline T getData(const T& default_return_val = T());
	//NOTE: compiler error c2770 (msvc2008) will result from using a reference type as template parameter
	//eg getDataWithCheck<int&>(bool&) will fail to compile.  This is a good thing, as GenericArgT<T&> don't exist anyway.
	//User pointers instead (GenericArgT<int*> rather than GenericArgT<int&>).
	template <class T>
	inline T getDataWithCheck(bool& failed, const T& default_return_val = T());
	template <class T>
	inline typename add_ptr<T>::type getDataPtr();
	
};
template <class T>
class GenericArgT : public GenericArgBase
{
public:
	GenericArgT(const T& data):_data(data){}
	T getData(){return _data;}
	typename remove_ref<T>::type* getDataPtr(){return &_data;}
	GenericArgBase* clone(){return new GenericArgT(_data);}
	const char* getTypeid(){return _typeid;}
	std::string stringifyData()
	{
		std::ostringstream o;
		if( ! (o<<_data) )
		{
			o<<"stringifyData(type="<<getTypeid()<<") failed.";
		}
		return o.str();
	}
protected:
	T _data;
	static const char* _typeid;
};
template <class T> const char* GenericArgT<T>::_typeid = typeid(T).name();

template <class T>
bool GenericArgBase::test()
{
	return dynamic_cast<GenericArgT<T>*>(this) != 0;
}
template <class T>
bool GenericArgBase::getDataByRef(T& data)
{	
	typedef remove_ref<T>::type unref_type;
	GenericArgT<unref_type>* arg = dynamic_cast<GenericArgT<unref_type>*>(this);
	if(!arg) return false;
	data = arg->getData();
	return true;
}
//NOTE: compiler error c2770 (msvc2008) will result from using a reference type as template parameter
//eg getData<int&>() will fail to compile.  This is a good thing, as GenericArgT<T&> don't exist anyway.
//User pointers instead (GenericArgT<int*> rather than GenericArgT<int&>).
template <class T>
T GenericArgBase::getData(const T& default_return_val)
{
	GenericArgT<T>* arg = dynamic_cast<GenericArgT<T>*>(this);
	if(!arg) return default_return_val;
	return arg->getData();
}
//NOTE: compiler error c2770 (msvc2008) will result from using a reference type as template parameter
//eg getData<int&>() will fail to compile.  This is a good thing, as GenericArgT<T&> don't exist anyway.
//User pointers instead (GenericArgT<int*> rather than GenericArgT<int&>).
template <class T>
T GenericArgBase::getDataWithCheck(bool& failed, const T& default_return_val)
{
	GenericArgT<T>* arg = dynamic_cast<GenericArgT<T>*>(this);
	failed = arg==0;
	if(!arg) return default_return_val;
	return arg->getData();
}
template <class T>
typename add_ptr<T>::type GenericArgBase::getDataPtr()
{
	if(is_ref<T>::value)
	{
		GenericArgT<add_ptr<T>::type>* arg = dynamic_cast<GenericArgT<add_ptr<T>::type>*>(this);
		if(!arg) return 0;
		return *arg->getDataPtr();
	}
	else
	{
		GenericArgT<remove_ref<T>::type>* arg = dynamic_cast<GenericArgT<remove_ref<T>::type>*>(this);
		if(!arg) return 0;
		return arg->getDataPtr();
	}
	
}

class GenericArg
{
public:
	GenericArg():_arg(0){}
	template<class T>
	inline GenericArg(T t):_arg(new GenericArgT<T>(t)){}
	template<>
	inline GenericArg(const char* str):_arg(new GenericArgT<std::string>(std::string(str))){}
	GenericArg(const GenericArg& other):_arg(0)
	{
		if(other.valid()) 
		{
			_arg = other._arg->clone();
		}
	}
	~GenericArg(){delete _arg;}
	const char* getTypeid(){if(!_arg)return "invalid_generic_arg"; return _arg->getTypeid();}
	std::string stringifyData(){if(!_arg) return "empty generic arg"; return _arg->stringifyData();}
	inline bool valid() const {return _arg!=0;}
	template<class T>
	inline GenericArg& operator=(const T& rhs)
	{
		delete _arg;
		_arg = 0;
		GenericArg temp(rhs);
		if(temp.valid()) _arg = temp._arg->clone();
		return *this;
	}

	GenericArg& operator=(const GenericArg& rhs)
	{
		delete _arg;
		_arg = 0;
		if(rhs.valid()) _arg = rhs._arg->clone();
		return *this;
	}



	template <class T>
	inline bool test()
	{
		if(!_arg) return false;
		return _arg->test<T>();
	}
	template <class T>
	inline bool getDataByRef(T& data)
	{
		if(!_arg) return false;
		return _arg->getDataByRef<T>(data);
	}
	template <class T>
	inline T getData(const T& default_return_val = T())
	{
		if(!_arg) return default_return_val;
		return _arg->getData<T>(default_return_val);
	}
	template <class T>
	inline T getDataWithCheck(bool& failed, const T& default_return_val = T())
	{
		if(!_arg){failed = true; return default_return_val;}
		return _arg->getDataWithCheck<T>(failed,default_return_val);
	}
	template <class T>
	inline typename add_ptr<T>::type getDataPtr()
	{
		if(!_arg) return 0;
		return _arg->getDataPtr<T>();
	}
	
private:
	GenericArgBase* _arg;
};



class GenericArgList
{
public:
	GenericArgList(){}
	~GenericArgList(){for(unsigned int i = 0;i<argList.size();++i) delete argList[i];}
	template<class T>
	inline void pushArg(const T& data){pushArg(GenericArg(data));}
	template<>
	void pushArg(const GenericArg& arg){argList.push_back(new GenericArg(arg));}
	
	inline unsigned int getNumArgs() const {return argList.size();}
	GenericArg* argAt(unsigned int pos) const
	{
		if(pos<argList.size()) return argList[pos];
		return 0;
	}
	const char* getTypeidAt(unsigned int pos) const {if(pos<argList.size())return argList[pos]->getTypeid(); return "out of range error";}
	template<class T>
	inline bool testAt(unsigned int pos) const
	{
		if(pos<argList.size()) return argList[pos]->test<T>();
		return false;
	}
	template<class T>
	inline T getDataAt(unsigned int pos, const T& default_return_val = T()) const 
	{
		if(pos<argList.size()) return argList[pos]->getData<T>(default_return_val);
		return default_return_val;
	}
	template<class T>
	inline T getDataAtWithCheck(unsigned int pos, bool& failed, const T& default_return_val = T())const 
	{
		if(pos<argList.size()) return argList[pos]->getDataWithCheck<T>(failed,default_return_val);
		failed = true;
		return default_return_val;
	}
protected:
	std::vector<GenericArg*> argList;
};