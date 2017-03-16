project(Communication CXX)
cmake_minimum_required(VERSION 2.8)
#Add all needed files
include_directories(. .. ${MESOS_3RD_PARTY}/boost-1.53.0 ${PROJECT_3RD_LOC})
set(MYSources CommandsImpl.cpp DBClient.cpp GrpcServer.cpp Serializor.cpp MessageType.cpp Serializor.cpp ./GeneretedFiles/ClusterService.grpc.pb.cc ./GeneretedFiles/ClusterService.pb.cc)
message(STATUS "CXX files - ${MYSources}")
#Pre build step
add_custom_target(HI_REDIS_BUILD COMMAND make --directory=${PROJECT_3RD_LOC}/hiredis-0.13.3/ OPTIMIZATION=)
add_custom_target(COPY_REDIS_PRODUCTS COMMAND mv ${PROJECT_3RD_LOC}/hiredis-0.13.3/libhiredis.a ${EXECUTABLE_OUTPUT_PATH})
#Compile staticly
add_library(Communication STATIC ${MYSources})
add_dependencies(Communication COPY_REDIS_PRODUCTS HI_REDIS_BUILD)
