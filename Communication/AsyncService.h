#pragma once

#include <memory>
#include "grpc++/grpc++.h"
#include "GeneretedFiles/TextualSearchResultsService.grpc.pb.h"
#include "Core/Exception.h"

class AsyncService
{
public:
    enum class State{
        NotConnected,
        Connected,
        Completed
    };

public:
    AsyncService():m_state(State::NotConnected){}
    State GetState(){
        return m_state;
    }

protected:
    std::shared_ptr<grpc::Service> m_service;
    State m_state;

private:
    friend class GrpcServer;
    grpc::Service* GetService(){
        return m_service.get();
    }
    virtual void Connect(void* tag){
        core::Exception(SOURCE, "Connect is not supported");
    }
    virtual void Invoke(void* tag){
        core::Exception(SOURCE, "Invoke is not supported");
    }
};

