#define SRCFILES - these are the .cpp and .ui files
set(SRCFILES
	#################.cpp files
	./FunctionQueueQT.cpp
	./GeneralGui.cpp
	./NerveAPI.cpp
	./NerveApplication.cpp
	./PluginHandler.cpp
	./PluginRegistry.cpp
	./Rebroadcast.cpp
	./RebroadcastManager.cpp
	./SettingsEditor.cpp
	./Task.cpp
	./TaskAdapter.cpp
	./TaskAdapterGui.cpp
	./TaskRegistry.cpp
	#################.ui files
	./GeneralGui.ui
	./SettingsEditor.ui
	./TaskAdapterGui.ui
	)#end definition of SRCFILES
set(UIFILES
	./GeneralGui.ui
	./SettingsEditor.ui
	./TaskAdapterGui.ui
	)#end definition of UIFILES
set(HDRFILES
	)#end definition of HDRFILES

set(HDRS_TO_MOC
	../../include/nrvApp/GeneralGui.h 
	../../include/nrvApp/SettingsEditor.h
	../../include/nrvApp/TaskAdapterGui.h
	../../include/nrvToolbox/RebroadcastQtAdapter.h
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
include_directories("C:/Users/Tom/NERVE/include")

#directories for the Properties->Linker->General->
#   Additional Library Directories field
link_directories()

#create the project to make the NerveApp library
#the first argument is the name of the project that will be created
#SHARED means it is a dynamic link library (Windows, .dll filetype) 
#source files come next
add_library(NerveApp SHARED ${SRCFILES} ${HEADERFILES})

#add libraries to link against
target_link_libraries(NerveApp ${QT_LIBRARIES})
target_link_libraries(NerveApp ${OPENSCENEGRAPH_LIBRARIES})
target_link_libraries(NerveApp winmm)

#add definitions
add_definitions(-D_UNICODE 
		-DBUILD_NERVEAPPLICATION_LIBRARY
		-DBUILD_NERVETASK_LEGACY_LIBRARY
		)
#set compiler flags
set_target_properties(NerveApp
	PROPERTIES COMPILE_FLAGS
		"/Zc:wchar_t-"
	)

##############################################################
#create the project to make the NerveApp executable
#the first argument is the name of the project that will be created
#SHARED means it is a dynamic link library (Windows, .dll filetype) 
#source files come next
#add_executable(NerveApp SHARED ${SRCFILES} ${HEADERFILES})

#files for the Properties->Linker->Input->
#   Additional Dependencies field
#the first argument is the name of the project this applies to
#then come the filenames
#target_link_libraries(tutorial nrvAppLib.lib)

