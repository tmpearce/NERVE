cmake_minimum_required(VERSION 2.8)
PROJECT(Nerve)

set(APPLICATION_NAME Nerve)
set(SRCFILES
	${APPLICATION_NAME}.cpp
	)

link_directories($ENV{NERVE_LIB_DIR})
include_directories(../include)

find_package(OpenSceneGraph)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIR})

add_executable(${APPLICATION_NAME} ${SRCFILES})
target_link_libraries(${APPLICATION_NAME} NerveApp)