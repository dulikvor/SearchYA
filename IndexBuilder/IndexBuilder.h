#pragma once

#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "Communication/DBClient.h"
#include "Core/src/TimedAsyncExecutor.h"
#include "ProcessingManager.h"
#include "Executor.h"

class DBClient;
struct Command;

class IndexBuilder
{
public:
	static IndexBuilder& Instance();
	//Waits indefinitely till the instance running state will be changed to false.
	void WaitForCompletion();
	void Terminate();
	void InitializeMesos();

	//Accessors
 	ProcessingManager& GetProcessingManager() {return m_processingManager;}
	Executor& GetExecutor() {return m_executor;}
	DBClient& GetDBClient() {return m_dbClient;}

private:
	IndexBuilder():m_processingManager(m_executor), m_terminate(false){}
	~IndexBuilder(){}

private:
	Executor m_executor;
	ProcessingManager m_processingManager;
	DBClient m_dbClient;
    mutable std::condition_variable m_conditionVar;
	mutable std::mutex m_mutex;
	std::atomic_bool m_terminate;
};
