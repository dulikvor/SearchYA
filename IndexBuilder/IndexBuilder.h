#pragma once

#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "Communication/DBClient.h"
#include "Core/TimedAsyncExecutor.h"
#include "StateContext.h"
#include "ProcessingManager.h"
#include "Executor.h"

class DBClient;
struct Command;

class IndexBuilder
{
public:
	static IndexBuilder& Instance();
	//Waits indefinitely till the instance running state will be changed to false.
	void NewCommand(CommandType commandType, const GeneralParams &params);
	void WaitForCompletion();
	void Init(const GeneralParams& params);
	void Terminate();
	void Processing(const GeneralParams& params);
	void InitializeMesos();

	//Accessors
 	ProcessingManager& GetProcessingManager() {return m_processingManager;}
	Executor& GetExecutor() {return m_executor;}
	DBClient& GetDBClient() {return m_dbClient;}

private:
    enum class RunningState
    {
        POWERED,
		INITIALIZE,
        TERMINATED
    };

	IndexBuilder();
	~IndexBuilder();
	//Will flush the log and change the instance internal state into not running 
	//(will cause the original thread to end).
	//
	void HandleCommand(const Command& command);
private:
	Executor m_executor;
	StateContext m_stateMachine;
	ProcessingManager m_processingManager;
	core::AsyncExecutor m_asyncExecutor;
	DBClient m_dbClient;
	RunningState m_state;
    mutable std::condition_variable m_conditionVar;
	mutable std::mutex m_mutex;
};
