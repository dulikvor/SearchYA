#pragma once

#include <vector>
#include <list>
#include <mutex>
#include "Core/AsyncExecutor.h"
#include "Core/ConcurrentDictionary.h"
#include "TaskType.h"

namespace core
{
	class AsyncTask;
}

class Task;
class Executor;

class ProcessingManager
{
public:
	using TaskID = std::string;
	ProcessingManager(Executor& executor);
	~ProcessingManager(){}
	//Will create a and start the run of a newly created task.
	//The manager will also keep track of the newly created task using its recieved id.
	//if the task already exists an exception will be thrown.
	void SubmitNewTask(TaskType taskType,
        const std::string& taskID,  float coreCount, const char* data, int length);

private:
	void RunNewTask(Task* task);
	void OnTaskCompletion(Task* task);
	std::shared_ptr<core::AsyncTask> CreateAsyncTask(TaskType taskType, std::unique_ptr<Task> task);

private:
	mutable std::mutex m_completionMut;
	Executor& m_executor;
	std::list<std::shared_ptr<core::AsyncTask>> m_runningAsyncTasks;
	core::AsyncExecutor m_asyncExecutor;
};
