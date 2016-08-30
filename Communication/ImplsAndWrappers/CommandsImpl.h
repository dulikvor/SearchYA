#pragma once

#include "GeneretedFiles/ClusterService.grpc.pb.h"

class CommandsImpl : public ClusterService::Commands::Service
{
public:
   ::grpc::Status LoadParams(::grpc::ServerContext* context, const ::ClusterService::Params* request, ::ClusterService::Empty* response) override;
};
