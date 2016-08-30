#include "CommandsImpl.h"
#include "ClusterManager/Application/ClusterManager.h"
#include "ClusterManager/Application/CommandType.h"
#include "ClusterManager/Params/Params.h"

using namespace grpc;

Status CommandsImpl::LoadParams(ServerContext* context, const ClusterService::Params* request, ClusterService::Empty* response)
{
    Params nativeParams;
    nativeParams.Load(*request);
    ClusterManager::Instace().NewCommand(CommandType::LoadParams, nativeParams);
}

