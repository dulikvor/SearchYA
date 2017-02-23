#pragma once

#include <memory>
#include <condition_variable>
#include <mutex>
#include "Core/AsyncExecutor.h"
#include "StateContext.h"
#include "Scheduler.h"

struct Command;

class ClusterManager
{
public:
    static ClusterManager& Instace();
    virtual ~ClusterManager();
	//NewCommand will divert the newly received GRPC command handling from the GRPC thread to the ClusterManager internal
	//async executor thread pool, the operation is done in sync manner.
    void NewCommand(CommandType commandType, const Params& params);
	//WaitForCompletion will block the calling thread under condition the ClusterManager state didn't reached terminate.
    void WaitForCompletion();
    void Terminate();
	//Init routine will do the followings:
	//1) Initializing the scheduler.
	void Init();
	//
	void InitializeMesosDriver();
	//Accessors
	Scheduler& GetScheduler(){ return m_scheduler; }

public:
    enum class State
    {
        RUNNING,
        TERMINATED
    };

private:
	//HandleCommand will handle a newly received GRPC command, the function will address the ClusterManager
	//state machine, providing it the responsibility to handle the newly received command under the current state.
    void HandleCommand(const Command& command);
    ClusterManager();

private:
	core::AsyncExecutor m_asyncExecutor;
    StateContext m_stateMachine;
    State m_state;
	Scheduler m_scheduler;
    mutable std::condition_variable m_conditionVar;
    mutable std::mutex m_mutex;
};
