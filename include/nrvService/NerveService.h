#pragma once
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
class InvocableInfo
{
public:
	InvocableInfo(const std::string& name, const std::string& returnType, unsigned int num_args,const std::string& description = "")
		:_name(name),_returnType(returnType),_description(description),_num_args(num_args),_arg_insert_pos(0),invalid_arg_num("Invalid value for arg_num")
	{
		_arg_type.resize(num_args);
		_arg_name.resize(num_args);
		_arg_description.resize(num_args);
	} 
	const std::string& getName() const {return _name;}
	const std::string& getDescription() const {return _description;}
	std::string getSignature() const
	{
		std::string str = _returnType;
		str.append(" ").append(_name).append("(");
		if(valid())
		{
			for(unsigned int i=0; i<_num_args;++i)
			{
				if(i>0) str.append(", ");
				str.append(_arg_type[i]).append(" ").append(_arg_name[i]);
			}
			str.append(");");
		}
		else
		{
			str.append(") invalid: not all args have been added.");
		}
		return str;
	}
	const std::string& getReturnType() const {return _returnType;}
	unsigned int       getNumArgs() const {return _num_args;}
	bool               getArgInfo(unsigned int arg_num, std::string& type, std::string& name, std::string& description) const
	{
		if(arg_num>=_num_args)return false;
		type = _arg_type[arg_num];
		name = _arg_name[arg_num];
		description = _arg_description[arg_num];
		return true;
	}
	bool checkArgType(unsigned int arg_num, const std::string& type) const
	{
		if(arg_num>=_num_args)return false;
		return _arg_type[arg_num].compare(type)==0;
	}
	template<class T>
	inline bool checkArgType(unsigned int arg_num) const
	{
		return _arg_type[arg_num].compare(typeid(T).name())==0;
	}
	bool checkArgName(unsigned int arg_num, const std::string& name) const
	{
		if(arg_num>=_num_args)return false;
		return _arg_name[arg_num].compare(name)==0;
	}
	
	const std::string& getArgType(unsigned int arg_num) const
	{
		if(arg_num>=_num_args)return invalid_arg_num;
		return _arg_type[arg_num];
	}
	const std::string& getArgName(unsigned int arg_num) const
	{
		if(arg_num>=_num_args)return invalid_arg_num;
		return _arg_name[arg_num];
	}
	const std::string& getArgDescription(unsigned int arg_num) const
	{
		if(arg_num>=_num_args)return invalid_arg_num;
		return _arg_description[arg_num];
	}

	bool pushArg(const std::string& type, const std::string& name, const std::string& description = "")
	{
		if(_arg_insert_pos>=_num_args) return false;
		_arg_type[_arg_insert_pos] = type;
		_arg_name[_arg_insert_pos] = name;
		_arg_description[_arg_insert_pos] = description;
		++_arg_insert_pos;
		return true;
	}
	template<class T>
	inline bool pushArg(const std::string& name, const std::string& description = "")
	{
		if(_arg_insert_pos>=_num_args) return false;
		_arg_type[_arg_insert_pos] = typeid(T).name();
		_arg_name[_arg_insert_pos] = name;
		_arg_description[_arg_insert_pos] = description;
		++_arg_insert_pos;
		return true;
	}
	
private:
	std::string _name;
	std::string _returnType;
	std::string _description;
	unsigned int _num_args;
	unsigned int _arg_insert_pos;
	std::vector<std::string> _arg_type;
	std::vector<std::string> _arg_name;
	std::vector<std::string> _arg_description;
	std::string invalid_arg_num;

	bool valid() const {return _arg_insert_pos==_num_args;}
	InvocableInfo():_name("__SFIDefaultCtorInvalidFunc_"),_description("Invalid function"),_num_args(0),_arg_insert_pos(1){}
	friend class ServiceMetaInfo;
};
class ServiceMetaInfo
{
public:
	ServiceMetaInfo(const std::string& name,std::vector<InvocableInfo>& invocableList, const std::string& description = "" ):
		_name(name),_description(description),_valid(true)
	{
		for(unsigned int i=0;i<invocableList.size();++i)
		{
			if(invocableList[i].valid()){ invocables.push_back(invocableList[i]); }
			else
			{
				_valid = false;
				printf("Warning: ServiceMetaInfo(%s): invalid function detected (%s). Not added to MetaInfo container.\n",
					_name.c_str(),invocableList[i].getName().c_str());
			}
		}
	}
	ServiceMetaInfo(const std::string& name, const std::string& description = "" ):_name(name),_description(description),_valid(true){}
	
	static std::string metaInfoAsString(const ServiceMetaInfo& metaInfo)
	{
		std::ostringstream o;
		o<<"ServiceMetaInfo:\n";
		o<<"Name: "<<metaInfo.getName()<<"\n";
		o<<"Description: "<<metaInfo.getDescription()<<"\n";
		o<<"Invocable functions:\n";
		for(unsigned int i=0;i<metaInfo.getNumInvocables();++i)
		{
			o<<metaInfo.getInvocable(i).getSignature()<<"\n";
		}
		o<<"\n";
		return o.str();
	}

	const std::string& getName() const {return _name;}
	const std::string& getDescription() const {return _description;}
	unsigned int getNumInvocables() const {return invocables.size();}
	bool isValid() const {return _valid;}
	const InvocableInfo& getInvocable(unsigned int i) const
	{
		if(i<invocables.size()) return invocables[i];
		else return invalid_func;
	}
	void pushInvocable(InvocableInfo info)
	{
		if(info.valid()){ invocables.push_back(info); }
		else
		{
			_valid = false;
			printf("Warning: ServiceMetaInfo(%s): invalid function detected (%s). Not added to MetaInfo container.\n",
				_name.c_str(),info.getName().c_str());
		}
	}
	ServiceMetaInfo():_name("__SMIDefaultCtorInvalidMetaInfo_"),_description("No description provided"),_valid(false){}
	void setName(const std::string& name){_name = name;_valid = true;}
	void setDescription(const std::string& desc){_description = desc;}
private:
	std::string _name;
	std::string _description;
	std::vector<InvocableInfo> invocables;
	InvocableInfo invalid_func;
	bool _valid;
};