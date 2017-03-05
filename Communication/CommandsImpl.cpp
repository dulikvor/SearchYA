#include "CommandsImpl.h"
#include "Core/Logger.h"
#include "ClusterManager/ClusterManager.h"
#include "ClusterManager/CommandType.h"
#include "ClusterManager/Params.h"

using namespace grpc;

Status CommandsImpl::Init(ServerContext* context, const ClusterService::Params* request, ClusterService::Empty* response)
{
    Params nativeParams;
    nativeParams.Load(*request);
    ClusterManager::Instace().NewCommand(CommandType::Init, nativeParams);
	return Status::OK;
}


Status CommandsImpl::SubmitTask(ServerContext* context, const ClusterService::Empty* request, ClusterService::Empty* response)
{
	ClusterManager::Instace().NewCommand(CommandType::Process, Params());
	return Status::OK;
}

Status CommandsImpl::Terminate(ServerContext* context, const ClusterService::Empty* request, ClusterService::Empty* response)
{

	ClusterManager::Instace().NewCommand(CommandType::Terminate, Params());
	return Status::OK;
}
