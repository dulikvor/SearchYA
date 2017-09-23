#pragma once

#include <memory>
#include "grpc++/grpc++.h"
#include "Core/Exception.h"

class GrpcServer;

class Service
{
public:
    virtual ~Service(){}

protected:

private:
    friend class GrpcServer;
    virtual grpc::Service* GetService(){
        throw core::Exception(SOURCE, "Non supported function");
    }
};