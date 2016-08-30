#pragma once

#include <memory>
#include <condition_variable>
#include <mutex>
#include "Thread/AsyncExecutor.h"
#include "StateContext.h"

struct Command;

class ClusterManager
{
public:
    static ClusterManager& Instace();
    virtual ~ClusterManager();

    void NewCommand(CommandType commandType, const Params& params);
    void WaitForCompletion();
    void Terminate();

public:
    enum class State
    {
        RUNNING,
        TERMINATED
    };

private:
    void HandleCommand(const Command& command);
    ClusterManager();

private:
    AsyncExecutor m_asyncExecutor;
    StateContext m_stateMachine;
    State m_state;
    mutable std::condition_variable m_conditionVar;
    mutable std::mutex m_mutex;
};
