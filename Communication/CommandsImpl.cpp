#include "CommandsImpl.h"
#include "ClusterManager/ClusterManager.h"
#include "ClusterManager/CommandType.h"
#include "ClusterManager/Params.h"

using namespace grpc;

Status CommandsImpl::Init(ServerContext* context, const ClusterService::Params* request, ClusterService::Empty* response)
{
    Params nativeParams;
    nativeParams.Load(*request);
    ClusterManager::Instace().NewCommand(CommandType::Init, nativeParams);
}

