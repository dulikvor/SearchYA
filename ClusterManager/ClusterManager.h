#pragma once

#include <condition_variable>
#include <mutex>
#include "Communication/ITextualSearchService.h"
#include "Core/AsyncExecutor.h"
#include "Communication/GrpcServer.h"
#include "StateContext.h"
#include "Scheduler.h"

struct Command;
class GrpcServer;
class GeneralParams;

class ClusterManager : public ITextualSearchService
{
public:
    static ClusterManager& Instace();
    virtual ~ClusterManager();
   	void Init(const GeneralParams& params) override;
   	void IndexDocument(const GeneralParams& params) override;
	void GetTopKDocuments(const std::string& word, int k, void* callbackTag) override;
   	void Terminate() override;
	//NewCommand will divert the newly received GRPC command handling from the GRPC thread 
	//to the ClusterManager internal async executor thread pool, the operation is done 
	//in sync manner.
    void NewCommand(CommandType commandType, const GeneralParams& params);
	//WaitForCompletion will block the calling thread under condition the ClusterManager 
	//state didn't reached terminate.
    void WaitForCompletion();
	//Will create and set grpc build server with a received listening point.
	//The server will be subscribed with needed services and start.
	void InitializeServer(const std::string& serverListeningPoint);
    void HandleTerminate();
	//Init routine will do the followings:
	//1) Initializing the scheduler.
	void HandleInit();
	//Accessors
	Scheduler& GetScheduler(){ return *m_scheduler; }
	GrpcServer& GetServer(){ return *m_server; }

public:
    enum class RunningState
    {
        RUNNING,
        TERMINATED
    };

private:
	//HandleCommand will handle a newly received GRPC command, the function will address 
	//the ClusterManager state machine, providing it the responsibility to handle 
	//the newly received command under the current state.
    void HandleCommand(const Command& command);
    ClusterManager();

private:
	core::AsyncExecutor m_asyncExecutor;
    StateContext m_stateMachine;
    RunningState m_state;
	std::unique_ptr<Scheduler> m_scheduler;
	std::unique_ptr<GrpcServer> m_server;
    mutable std::condition_variable m_conditionVar;
    mutable std::mutex m_mutex;
};
