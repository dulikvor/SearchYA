project(MesosBenchMark CXX)
cmake_minimum_required(VERSION 2.8)
#Set global settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS  "-std=c++11 -g -pthread -I/usr/local/include ")
#set(CMAKE_VERBOSE_MAKEFILE on)
#Pre build step
add_custom_target(PROTO_BUILD COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/Communication/IDL/BuildProto.bash ${CMAKE_CURRENT_SOURCE_DIR}/Communication)
#Add project dependencies
add_subdirectory(Core)
add_subdirectory(ClusterManager)
add_subdirectory(Communication)
#Link final product
include_directories(.)
add_executable(MesosBenchMark main.cpp)
add_dependencies(MesosBenchMark PROTO_BUILD)
set(STANDARD_LIBS pthread protobuf mesos)
target_link_libraries(MesosBenchMark ClusterManager Communication Core ${STANDARD_LIBS})
