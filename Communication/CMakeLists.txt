project(Communication CXX)
cmake_minimum_required(VERSION 2.8)
#Add all needed files
include_directories(. ..)
file(GLOB_RECURSE MYSources *.cpp *.cc)
message(STATUS "CXX files - ${MYSources}")
#Compile staticly
add_library(Communication STATIC ${MYSources})
