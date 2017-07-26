project(IndexBuilder CXX)
cmake_minimum_required(VERSION 2.8)
#Set global settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)
set(CMAKE_BINARY_DIR ${PROJECT_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)
set(PROJECT_3RD_LOC ${CMAKE_CURRENT_SOURCE_DIR}/../Third_Party)
set(MESOS_3RD_PARTY "~/Downloads/mesos/")
set(CMAKE_CXX_FLAGS  "-std=c++11 -g -pthread -I/usr/local/include ")
#Add all needed files
include_directories(. .. ${PROJECT_3RD_LOC})
set(MYSources main.cpp IndexBuilder.cpp TaskType.cpp Executor.cpp TaskState.cpp ProcessingManager.cpp IndexingTask.cpp InitTask.cpp InitTask.h TasksFactoryContainer.cpp TasksFactoryContainer.h TaskCreator.cpp TaskCreator.h)
add_executable(IndexBuilder ${MYSources})
#Add project dependencies
add_subdirectory(${PROJECT_DIR}/../Core ${PROJECT_DIR}/Core) 
add_subdirectory(${PROJECT_DIR}/../Communication ${PROJECT_DIR}/Communication)
#Link final product
set(TO_LINK_LIBS protobuf mesos ${EXECUTABLE_OUTPUT_PATH}/libhiredis.a ${PROJECT_DIR}/../RedisSearchModule/bin/libSearchModule_s.a)
target_link_libraries(IndexBuilder Communication Core ${TO_LINK_LIBS})
