#pragma once

#include <list>
#include <mutex>
#include "Core/AsyncExecutor.h"
#include "Core/ConcurrentDictionary.h"

namespace core
{
	class AsyncTask;
}

class Task;

class ProcessingManager
{
public:
	using TaskID = std::string;
	ProcessingManager();
	~ProcessingManager();
	//Will create a and start the run of a newly created task.
	//The manager will also keep track of the newly created task using its recieved id.
	//if the task already exists an exception will be thrown.
	void SubmitNewTask(const std::string& taskID, int coreCount);
	//Will spawn a newly received task activity into execution using the stored async executor.
	void SubmitTaskActivity(core::AsyncTask* activity);
	//Will receive a list of computation activities originates from some task.
	//all activities will be spawned upon the computation async executor.
	//along will be also added a core count, depicting the resource requierment of the received
	//activities. they will be use to increase the utilization of the computation 
	//async executor.
	void SubmitProcessingActivities(std::list<core::AsyncTask*> activities,
		   int coreCount);
	//Will inform the processing manager with the completion of a given task.
	//due to race condition with various processing activities of many diffrent tasks,
	//the method is sync. due to that an order of completion tasks and thier equivalent
	//async tasks (which carry the operation) are matched. the last published task
	//is the last one on the list.
	void SubmitTaskCompletion(const std::string& taskID);
	//Will be called upon task completion, will remove and delete the given task.
	//Will delete the previous completion async task and replace it with the current async task.
	void OnTaskCompletion(const std::string taskID);
	//If exists, will return a requested task (according to the received task id).
	Task* GetTask(const TaskID& taskID);

private:
	mutable std::mutex m_completionMut;
	std::list<core::AsyncTask*> m_completionAsyncTasks;
	core::ConcurrentDictionary<TaskID, Task*> m_tasks;
	core::AsyncExecutor m_executor;
	std::unique_ptr<core::AsyncExecutor> m_computationAsyncExecutor;
	core::AsyncTask* m_taskCompletion;
};
