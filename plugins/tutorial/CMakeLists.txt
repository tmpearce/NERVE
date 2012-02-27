cmake_minimum_required(VERSION 2.8)
PROJECT(tutorial)

set(PLUGIN_FILENAME tutorial)
set(SRCFILES
	${PLUGIN_FILENAME}.cpp
	)

link_directories($ENV{NERVE_LIB_DIR})
include_directories(../../include)

add_library(${PLUGIN_FILENAME} SHARED ${SRCFILES})
target_link_libraries(${PLUGIN_FILENAME} NerveApp)