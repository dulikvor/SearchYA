project(MesosBenchMark CXX)
cmake_minimum_required(VERSION 2.8)
#Set global settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)
set(CMAKE_BINARY_DIR ${PROJECT_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)
set(PROJECT_3RD_LOC ${CMAKE_CURRENT_SOURCE_DIR}/3RD_Party)
set(CMAKE_CXX_FLAGS  "-std=c++11 -g -pthread -I/usr/local/include ")
#set(CMAKE_VERBOSE_MAKEFILE on)
#Pre build step
add_custom_target(PROTO_BUILD COMMAND ${PROJECT_DIR}/Communication/IDL/BuildProto.bash ${PROJECT_DIR}/Communication)
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
