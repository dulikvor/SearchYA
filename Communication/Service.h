#pragma once

#include <memory>
#include "GeneretedFiles/TextualSearchResultsService.grpc.pb.h"
#include "Core/AutoResetEvent.h"

class GrpcServer;

class Service
{
public:
    enum class State{
        NotCompleted,
        Completed
    };
public:
    Service():m_state(State::NotCompleted){}
    virtual ~Service(){}

protected:
    std::shared_ptr<grpc::Service> m_service;
    core::AutoResetEvent m_connectedEvent;
    State m_state;

private:
    friend class GrpcServer;
    grpc::Service* GetService(){
        return m_service.get();
    }
    void SetConnected();
    State GetState(){
        return m_state;
    }
};