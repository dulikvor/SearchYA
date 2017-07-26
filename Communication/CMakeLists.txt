project(Communication CXX)
cmake_minimum_required(VERSION 2.8)
#Add all needed files
include_directories(. .. ${MESOS_3RD_PARTY}/boost-1.53.0 ${PROJECT_3RD_LOC})
set(MYSources TextualSearchServiceImpl.cpp DBClient.cpp GrpcServer.cpp Serializor.cpp MessageType.cpp Serializor.cpp ./GeneretedFiles/TextualSearchService.grpc.pb.cc ./GeneretedFiles/TextualSearchService.pb.cc DBClientReply.cpp)
message(STATUS "CXX files - ${MYSources}")
add_library(Communication STATIC ${MYSources})
#Pre link step
add_custom_command(TARGET Communication PRE_LINK 
	COMMAND make --directory=${PROJECT_3RD_LOC}/hiredis-0.13.3/ OPTIMIZATION=
	COMMAND mv ${PROJECT_3RD_LOC}/hiredis-0.13.3/libhiredis.a ${EXECUTABLE_OUTPUT_PATH})
