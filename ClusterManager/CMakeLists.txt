project(ClusterManager CXX)
cmake_minimum_required(VERSION 2.8)
#Add all needed files
include_directories(. .. ${PROJECT_3RD_LOC}/include ${MESOS_3RD_PARTY}/boost-1.53.0/ ${MESOS_3RD_PARTY}/glog-0.3.3/src/)
link_directories(${PROJECT_3RD_LOC}/lib)
set(MYSources main.cpp ClusterManager.cpp CommandType.cpp ConfigParams.cpp PowerUpState.cpp ProcessingState.cpp Scheduler.cpp State.cpp StateContext.cpp TaskState.cpp InitJob.cpp JobFactoryContainer.cpp JobType.cpp JobType.h )
add_executable(ClusterManager ${MYSources})
#Link final product
set(TO_LINK_LIBS pthread protobuf mesos grpc++)
target_link_libraries(ClusterManager Communication Core ${TO_LINK_LIBS})
