cmake_minimum_required(VERSION 2.8)
PROJECT(FileWriter)

set(PLUGIN_FILENAME FileWriter)
set(SRCFILES
	plugin.cpp
	IFileWriter.cpp
	)
set(HDRFILES
	IFileWriter.h
	)
link_directories($ENV{NERVE_LIB_DIR})
include_directories(../../include)

find_package(OpenSceneGraph 2.8.2 REQUIRED OpenThreads)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

add_library(${PLUGIN_FILENAME} SHARED ${SRCFILES} ${HDRFILES})
add_definitions(-DIFILEWRITER_EXPORT)
target_link_libraries(${PLUGIN_FILENAME} NerveApp ${OPENSCENEGRAPH_LIBRARIES})