project(ClusterManager CXX)
cmake_minimum_required(VERSION 2.8)
#Set global settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)
set(CMAKE_BINARY_DIR ${PROJECT_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)
set(PROJECT_3RD_LOC ${CMAKE_CURRENT_SOURCE_DIR}/../Thired_Party)
set(MESOS_3RD_PARTY "~/Downloads/mesos/")
set(CMAKE_CXX_FLAGS  "-std=c++11 -g -pthread -I/usr/local/include ")
#Pre build step
add_custom_target(PROTO_BUILD COMMAND ${PROJECT_DIR}/../Communication/IDL/BuildProto.bash ${PROJECT_DIR}/../Communication)
#Add project dependencies
add_subdirectory(${PROJECT_DIR}/../Core ${PROJECT_DIR}/Core) 
add_subdirectory(${PROJECT_DIR}/../Communication ${PROJECT_DIR}/Communication)
#Add all needed files
include_directories(. .. ${MESOS_3RD_PARTY}/boost-1.53.0)
set(MYSources main.cpp ClusterManager.cpp CommandType.cpp ConfigParams.cpp PowerUpState.cpp ProcessingState.cpp Scheduler.cpp State.cpp StateContext.cpp TaskState.cpp)
#Link final product
add_executable(ClusterManager ${MYSources})
add_dependencies(ClusterManager PROTO_BUILD)
set(TO_LINK_LIBS pthread protobuf mesos grpc++)
target_link_libraries(ClusterManager Communication Core ${TO_LINK_LIBS})
