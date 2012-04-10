#include "nrv/PluginDeveloperTools.h"
#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginFactory.h"
#include "nrvApp/NervePluginBase.h"

#include "IFileWriter.h"

M_MAKE_PLUGIN(FileWriterFactory)

class FileWriter : public NervePluginBase
{
public:
	void init(NerveAPI* n){mpAPI = n; mpAPI->exposeIPlugin(&iFileWriter);}
	~FileWriter(){mpAPI->hideIPlugin();}
private:
	NerveAPI* mpAPI;
	IFileWriter iFileWriter;
};

void FileWriterFactory::cleanUpPluginObject(NervePluginBase * p, NerveAPI * n)
{
	delete p;
}
NervePluginBase* FileWriterFactory::createPluginObject()
{
	return new FileWriter();
}
const char* FileWriterFactory::getName()
{
	return "FileWriter";
}