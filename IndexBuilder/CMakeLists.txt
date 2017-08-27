project(IndexBuilder CXX)
cmake_minimum_required(VERSION 2.8)
#Add all needed files
include_directories(. .. ${PROJECT_3RD_LOC})
set(MYSources main.cpp IndexBuilder.cpp TaskType.cpp Executor.cpp TaskState.cpp ProcessingManager.cpp IndexingTask.cpp InitTask.cpp InitTask.h TasksFactoryContainer.cpp TasksFactoryContainer.h TaskCreator.cpp TaskCreator.h)
add_executable(IndexBuilder ${MYSources})
#Link final product
set(TO_LINK_LIBS protobuf mesos ${PROJECT_3RD_LOC}/lib/libhiredis.a ${PROJECT_DIR}/../RedisSearchModule/bin/libSearchModule_s.a)
target_link_libraries(IndexBuilder Communication Core ${TO_LINK_LIBS})
