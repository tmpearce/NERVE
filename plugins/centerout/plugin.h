#pragma once
#include "nrvApp/NerveAPI.h"
#include "nrvApp/NervePluginBase.h"
#include "osgwindow/osgwindow_interface.h"
#include "FileWriter/IFileWriter.h"
#include "nrvThread/NerveThread.h"
#include "nrvThread/NerveModule.h"

#include <iomanip>

//files from this project
#include "plugin.h"
#include "GuiHeader.h"
#include "CenterOut.h"


class PositionModule : public NerveModule
{
	class BufferGetter : public NerveModule
public:
	PositionModule(CenterOut* cenout, IOSGWindow* window, float scale):co(cenout),w(window),s(scale),writeIndex(-1),bufferData(false),bufferRate(0.f){}
	void moduleOperation(NerveModuleUser*)
	{
		CenterOut::CursorPosition pos;
		IOSGWindow::Position mpos = w->getMousePosition();
		pos.x = mpos.first * s;
		pos.y = 0.f;
		pos.z = mpos.second * s;
		co->setCursorPosition(pos);
		OpenThreads::Thread::microSleep(100);
	}
	
private:
	CenterOut* co;
	IOSGWindow* w;
	float s;
};

class CenterOutPlugin : public NervePluginBase
{
public:
	enum CallbackID
	{
		CREATE_GUI,
		DESTROY_GUI
	};
	void init(NerveAPI* n)
	{
		mpAPI = n;
		mpAPI->callPluginFromMainThread(this,CREATE_GUI, NerveAPI::CALLBACK_REQUESTS_BLOCKING);
		posThread.start();

		createWindow();
		createOutputFile();
	}
	~CenterOutPlugin()
	{
		posThread.cancel();
		//clean up child plugins
		//first the osg window
		if(iWindow)
		{
			mpAPI->unbindIPlugin(iWindow);
		}
		mpAPI->cancelChildPlugin(window_plugin);
		//clean up the file writer child plugin
		if(iFile)
		{
			mpAPI->unbindIPlugin(iFile);
		}
		mpAPI->cancelChildPlugin(file_plugin);
		mpAPI->callPluginFromMainThread(this,DESTROY_GUI,NerveAPI::CALLBACK_REQUESTS_BLOCKING);
	}
	void callbackFromMainApplicationThread(int call_id)
	{
		switch(call_id)
		{
		case CREATE_GUI: createGui(); break;
		case DESTROY_GUI: destroyGui(); break;
		}
	}
	void acceptChildUI(QWidget* ui)
	{
		gui->acceptChildUI(ui);
	}

	void createWindow()
	{
		window_plugin = mpAPI->createPlugin("OSG Window <osgwindow.dll>");
		IPlugin* ip = mpAPI->bindIPlugin(window_plugin);
		iWindow = dynamic_cast<IOSGWindow*>(ip);
		/*if(iWindow)
		{
			printf("successfully cast IPlugin* to IOSGWindow*\n");
		}
		else
		{
			printf("failed to cast IPlugin* to IOSGWindow*\n");
		}*/
		iWindow->setScene(cenOut.getScene());
		iWindow->setCameraCallback(cenOut.getCameraCallback());
		iWindow->initWindowEvents();
		iWindow->setupInWindow();
		iWindow->applyChanges();

		PositionModule* m = new PositionModule(&cenOut, iWindow, 20.f);
		m->setOperateAction(NerveModule::DONT_REMOVE_MODULE);
		m->setRemoveAction(NerveModule::DELETE_MODULE);
		posThread.addModule(*m);
	}
	void createOutputFile()
	{
		file_plugin = mpAPI->createPlugin("FileWriter <FileWriter.dll>");
		IPlugin* ip = mpAPI->bindIPlugin(file_plugin);
		iFile = dynamic_cast<IFileWriter*>(ip);
		/*if(iFile)
		{
			printf("successfully cast IPlugin* to IFileWriter*\n");
		}
		else
		{
			printf("failed to cast IPlugin* to IFileWriter*\n");
		}*/
		iFile->openFile("centerout_data.txt");
		FileStream* fs = iFile->createFileStream();
		float buffer[5];
		for(int i=0;i<5;++i)
		{
			buffer[i] = (float)rand()/64000;
		}
		fs->filestr()<<"Testing FileStream"<<std::endl<<3.141590000<<std::endl<<std::setprecision(2)<<std::fixed;
		for(int i=0;i<5;++i) fs->filestr()<<buffer[i]<<" ";
		fs->flush();
	}
	void startTask(){cenOut.start();}
private:
	NerveThread posThread;
	NerveAPI* mpAPI;
	CenterOutGui* gui;
	IOSGWindow* iWindow;
	std::string window_plugin;
	IFileWriter* iFile;
	std::string file_plugin;
	CenterOut cenOut;

	void createGui()
	{
		gui=new CenterOutGui(this);
		mpAPI->exposeUI(gui);
		mpAPI->setTakeOwnershipOfCreatedPlugins(true);
		mpAPI->setWillAcceptChildUIs(true);
		
	}
	void destroyGui()
	{
		mpAPI->removeUI(gui);
		delete gui;
	}
};
