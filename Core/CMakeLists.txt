project(Core CXX)
cmake_minimum_required(VERSION 2.8)
#Add all needed files
include_directories(. ${PROJECT_3RD_LOC}/include)
file(GLOB_RECURSE MYSources *.cpp)
message(STATUS "CXX files - ${MYSources}")
#Compile staticly
add_library(Core STATIC ${MYSources} Deleter.h Process.cpp Process.h ChildProcess.cpp ChildProcess.h Pipe.cpp Pipe.h)
