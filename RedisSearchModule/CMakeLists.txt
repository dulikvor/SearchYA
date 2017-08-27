project(SearchModule CXX)
cmake_minimum_required(VERSION 2.8)
#Add all needed files
include_directories(. .. ${PROJECT_3RD_LOC}/include)
set(MYSources SearchModule.cpp Document.cpp DocumentsIndexer.cpp WordsTrieTree.cpp WordsTrieTreeNode.cpp)

add_library(objlib OBJECT ${MYSources})
#Compile staticly
add_library(SearchModule SHARED $<TARGET_OBJECTS:objlib>)
add_library(SearchModule_s STATIC $<TARGET_OBJECTS:objlib>)
target_link_libraries(SearchModule Communication Core)

