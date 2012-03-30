#cmakelists.txt for Nerve project
#This project is the base application, plugins are not included
#The project has a shared library (.dll on Windows)
#and an executable (.exe)

cmake_minimum_required(VERSION 2.8)
PROJECT(Nerve)

#define SRCFILES - these are the .cpp and .ui files
set(SRCFILES
	#################.cpp files
	./src/nrvApp/FunctionQueueQT.cpp
	./src/nrvApp/GeneralGui.cpp
	./src/nrvApp/NerveAPI.cpp
	./src/nrvApp/NerveApplication.cpp
	./src/nrvApp/PluginHandler.cpp
	./src/nrvApp/PluginRegistry.cpp
	./src/nrvApp/Rebroadcast.cpp
	./src/nrvApp/RebroadcastManager.cpp
	./src/nrvApp/SettingsEditor.cpp
	./src/nrvApp/Task.cpp
	./src/nrvApp/TaskAdapter.cpp
	./src/nrvApp/TaskAdapterGui.cpp
	./src/nrvApp/TaskRegistry.cpp
	./src/nrvApp/NervePluginFactory.cpp
	./src/nrvExe/NerveApp.cpp
	#################.ui files
	./src/nrvApp/GeneralGui.ui
	./src/nrvApp/SettingsEditor.ui
	./src/nrvApp/TaskAdapterGui.ui
	)#end definition of SRCFILES
set(UIFILES
	./src/nrvApp/GeneralGui.ui
	./src/nrvApp/SettingsEditor.ui
	./src/nrvApp/TaskAdapterGui.ui
	)#end definition of UIFILES
set(HDRFILES
	./include/nrv/EventObserver.h
	./include/nrv/MessageQueue.h
	./include/nrv/GuiTools.h
	./include/nrvApp/GeneralGui.h
	./include/nrvApp/NerveAPI.h
	./include/nrvApp/NerveApplication.h
	./include/nrvApp/NervePluginBase.h
	./include/nrvApp/NervePluginFactory.h
	./include/nrvApp/PluginHandler.h
	./include/nrvApp/PluginManager.h
	./include/nrvApp/PluginModule.h
	./include/nrvApp/PluginRegistry.h
	./include/nrvApp/ServiceRegistry.h
	./include/nrvApp/ServiceRepository.h
	./include/nrvApp/SettingsEditor.h
	./include/nrvApp/TaskAdapter.h
	./include/nrvApp/TaskAdapterGui.h
	./include/nrvApp/TaskRegistry.h
	./include/nrvExe/Task.h
	./include/nrvExe/NerveApp.h
	./include/nrvService/NerveService.h
	./include/nrvService/InvokeMapper.h
	./include/nrvService/Invoker.h
	./include/nrvService/ServiceBinding.h
	./include/nrvService/ServiceFactory.h
	./include/nrvThread/NerveThread.h
	./include/nrvThread/NerveModule.h
	./include/nrvThread/NerveModuleUser.h
	./include/nrvThread/BasicModuleUser.h
	./include/nrvToolbox/RebroadcastManager.h
	./include/nrvToolbox/FunctionQueueQt.h
	./include/nrvToolbox/Pause.h
	./include/nrvToolbox/Rebroadcast.h
	)#end definition of HDRFILES

set(HDRS_TO_MOC
	./include/nrvApp/GeneralGui.h 
	./include/nrvApp/SettingsEditor.h
	./include/nrvApp/TaskAdapterGui.h
	./include/nrvToolbox/RebroadcastQtAdapter.h
	)#end definition of HDRS_TO_MOC

#Deal with Qt stuff
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
qt4_wrap_ui(UI_HDRFILES ${UIFILES})
qt4_wrap_cpp(MOC_SRCFILES ${HDRS_TO_MOC})
include_directories(${CMAKE_BINARY_DIR})
list(APPEND SRCFILES ${MOC_SRCFILES})

#Deal with OpenSceneGraph stuff
find_package(OpenSceneGraph 2.8.2 REQUIRED OpenThreads)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})


#directories for the Properties->C/C++->General->
#   Additional Include Directories field
include_directories("./include")

#directories for the Properties->Linker->General->
#   Additional Library Directories field
link_directories()

#create the project to make the NerveApp library
#the first argument is the name of the project that will be created
#SHARED means it is a dynamic link library (Windows, .dll filetype) 
#source files come next
add_library(NerveApp SHARED ${SRCFILES} ${HDRFILES})

#add libraries to link against
target_link_libraries(NerveApp ${QT_LIBRARIES})
target_link_libraries(NerveApp ${OPENSCENEGRAPH_LIBRARIES})
target_link_libraries(NerveApp winmm)

#add definitions
add_definitions(-D_UNICODE 
		-DBUILD_NERVEAPPLICATION_LIBRARY
		-DBUILD_NERVETASK_LEGACY_LIBRARY
		-DNERVE_VERSION=3
		)
#set compiler flags
set_target_properties(NerveApp
	PROPERTIES COMPILE_FLAGS
		"/Zc:wchar_t-"
	)


