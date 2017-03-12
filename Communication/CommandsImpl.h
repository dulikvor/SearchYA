#pragma once

#include "GeneretedFiles/ClusterService.grpc.pb.h"

class CommandsImpl : public ClusterService::Commands::Service
{
public:
	virtual ~CommandsImpl(){}
   ::grpc::Status Init(::grpc::ServerContext* context, const ::ClusterService::Params* request, ::ClusterService::Empty* response) override;
   ::grpc::Status SubmitTask(::grpc::ServerContext* context, const ::ClusterService::Empty* request, ::ClusterService::Empty* response) override;
   	::grpc::Status Terminate(::grpc::ServerContext* context, const ::ClusterService::Empty* request, ::ClusterService::Empty* response) override;
};
