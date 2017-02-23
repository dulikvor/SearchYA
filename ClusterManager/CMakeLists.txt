project(ClusterManager CXX)
cmake_minimum_required(VERSION 2.8)
set(MESOS_3RD_PARTY "~/Downloads/mesos/build/3rdparty")
#Add all needed files
include_directories(. .. ${MESOS_3RD_PARTY}/boost-1.53.0)
file(GLOB_RECURSE MYSources *.cpp)
message(STATUS "CXX files - ${MYSources}")
#Compile staticly
add_library(ClusterManager STATIC ${MYSources})
