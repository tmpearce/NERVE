#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include "nrvThread/NerveThread.h"
#include "nrvThread/NerveModuleUser.h"
#include "IFileWriter.h"

class FileStream_private
{
public:
	FileStream_private(IFileWriter_private* writer):w_(writer){ostr = new std::ostringstream();}
	~FileStream_private(){delete ostr;}
	std::ostream& filestr(){return *ostr;}
	void flush();
private:
	IFileWriter_private* w_;
	std::ostringstream* ostr;
};
class FileStreamModule : public NerveModule
{
public:
	FileStreamModule(std::ostringstream* ostr, std::ofstream* ofstr):f(ofstr),s(ostr)
	{
		setOperateAction(NerveModule::REMOVE_MODULE_AFTER_OPERATING);
		setRemoveAction(NerveModule::DELETE_MODULE);
	}
	void moduleOperation(NerveModuleUser*){(*f)<<s->str();f->flush();delete s;}
	std::ostringstream* s;
	std::ofstream* f;
};
class Throttle : public NerveModule
{
public:
	Throttle()
	{
		setOperateAction(NerveModule::DONT_REMOVE_MODULE);
		setRemoveAction(NerveModule::DELETE_MODULE);
	}
	void moduleOperation(NerveModuleUser*){OpenThreads::Thread::microSleep(50000);}
};
class IFileWriter_private
{
public:
	bool openFile(const char* filename)
	{
		if(file.is_open() == false)
		{
			file.open(filename);
			thread.start();
			thread.addModule(*(new Throttle()));
			return true;
		}
		return false;
	}
	void closeCurrentFile()
	{
		thread.cancel();
		while(thread.isRunning()){OpenThreads::Thread::microSleep(10);}
		file.close();
	}
	FileStream* createFileStream()
	{
		if(file.is_open())
		{
			FileStream* fs = new FileStream(this);
			streams.insert(fs);
			return fs;
		}
		else
		{
			return 0;
		}
	}

	void write(std::ostringstream* s){thread.addModule(*(new FileStreamModule(s,&file)));}

	~IFileWriter_private()
	{
		closeCurrentFile();
		while(!streams.empty())
		{
			delete *(streams.begin());
			streams.erase(streams.begin());
		}
	}
private:
	std::ofstream file;
	NerveThread thread;
	std::set<FileStream*> streams;
};
FileStream::FileStream(IFileWriter_private* writer){d_ = new FileStream_private(writer);}
FileStream::~FileStream(){delete d_;}
std::ostream& FileStream::filestr(){return d_->filestr();}
void FileStream::flush(){return d_->flush();}
void FileStream_private::flush(){w_->write(ostr); ostr=new std::ostringstream();}
IFileWriter::IFileWriter(){d_ = new IFileWriter_private();}
IFileWriter::~IFileWriter(){delete d_;}
bool IFileWriter::openFile(const char* filename){return d_->openFile(filename);}
void IFileWriter::closeCurrentFile(){return d_->closeCurrentFile();}
FileStream* IFileWriter::createFileStream(){return d_->createFileStream();}