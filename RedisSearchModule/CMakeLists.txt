project(SearchModule CXX)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_3RD_LOC ${CMAKE_CURRENT_SOURCE_DIR}/../Third_Party)
set(MESOS_3RD_PARTY "~/Downloads/mesos/")
set(REDIS_LOC ${PROJECT_3RD_LOC}/redis)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)
set(CMAKE_CXX_FLAGS  "-std=c++11 -g -pthread -fPIC -I/usr/local/include ")
#Add all needed files
include_directories(. ..  ${PROJECT_3RD_LOC} ${REDIS_LOC}/src)
set(MYSources SearchModule.cpp Document.cpp DocumentsIndexer.cpp WordsTrieTree.cpp WordsTrieTreeNode.cpp)

add_library(objlib OBJECT ${MYSources})
#Compile staticly
add_library(SearchModule SHARED $<TARGET_OBJECTS:objlib>)
add_library(SearchModule_s STATIC $<TARGET_OBJECTS:objlib>)
#Add project dependencies
add_subdirectory(${PROJECT_DIR}/../Core ${PROJECT_DIR}/Core) 
add_subdirectory(${PROJECT_DIR}/../Communication ${PROJECT_DIR}/Communication)
#set_property(TARGET SearchModule PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(SearchModule Communication Core)

