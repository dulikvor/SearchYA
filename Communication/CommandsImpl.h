#pragma once

#include "GeneretedFiles/ClusterService.grpc.pb.h"

class CommandsImpl : public ClusterService::Commands::Service
{
public:
   ::grpc::Status Init(::grpc::ServerContext* context, const ::ClusterService::Params* request, ::ClusterService::Empty* response) override;
};
