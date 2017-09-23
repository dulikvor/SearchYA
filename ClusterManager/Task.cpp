#include "Task.h"
#include "ClusterManager.h"
#include "Communication/GrpcServer.h"

Task::Task(void * const tag) {
   m_service = ClusterManager::Instace().GetServer().GetAsyncService(tag);
}
