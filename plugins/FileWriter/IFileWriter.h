#pragma once
#include "nrvApp/NerveAPI.h"
#include <ostream>
#ifdef IFILEWRITER_EXPORT
#define IFILEWRITER __declspec(dllexport)
#else
#define IFILEWRITER __declspec(dllimport)
#endif
class IFileWriter_private;
class FileStream_private;

class IFILEWRITER FileStream
{
public:
	std::ostream& filestr();
	void flush();
private:
	friend class IFileWriter_private;
	FileStream(IFileWriter_private*);
	FileStream(const FileStream& other);
	~FileStream();
	FileStream_private* d_;
};
class IFILEWRITER IFileWriter : public IPlugin
{
public:
	bool openFile(const char* filename);
	void closeCurrentFile();
	FileStream* createFileStream();
private:
	friend class FileWriter;
	IFileWriter_private* d_;
	IFileWriter();
	IFileWriter(const IFileWriter& other);
	~IFileWriter();
};