project(Communication CXX)
cmake_minimum_required(VERSION 2.8)
#Add all needed files
include_directories(. .. ${PROJECT_3RD_LOC}/include)
set(MYSources TextualSearchServiceImpl.cpp DBClient.cpp GrpcServer.cpp Serializor.cpp MessageType.cpp Serializor.cpp ./GeneretedFiles/TextualSearchService.grpc.pb.cc ./GeneretedFiles/TextualSearchService.pb.cc ./GeneretedFiles/TextualSearchResultsService.grpc.pb.cc ./GeneretedFiles/TextualSearchResultsService.pb.cc DBClientReply.cpp TextualSearchResultsImpl.cpp TextualSearchResultsImpl.h Service.h Service.cpp)
message(STATUS "CXX files - ${MYSources}")
add_library(Communication STATIC ${MYSources})
