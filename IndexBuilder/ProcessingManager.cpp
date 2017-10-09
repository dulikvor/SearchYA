#include "ProcessingManager.h"
#include "Core/src/AsyncTask.h"
#include "Core/src/Enviorment.h"
#include "Communication/GeneralParams.h"
#include "RedisSearchModule/Document.h"
#include "TasksFactoryContainer.h"
#include "Executor.h"

using namespace std;
using namespace core;

ProcessingManager::ProcessingManager(Executor& executor)
	:m_asyncExecutor(Enviorment::Instance().GetCoreCount()), m_executor(executor)
{
}

void ProcessingManager::SubmitNewTask(TaskType taskType,
        const string& taskID,  float coreCount, const char* data, int length)
{
	TRACE_INFO("Task-%s: Creating", taskID.c_str());
	unique_ptr<Task> executorTask = TasksFactoryContainer::Instance().Create(
			taskType, taskID, coreCount, data, length);

	shared_ptr<AsyncTask> asyncTask = make_shared<AsyncTask>(bind(&ProcessingManager::RunNewTask, this, executorTask.release()));
	{
		unique_lock<mutex> lock(m_completionMut);
		m_runningAsyncTasks.push_back(asyncTask);
	}
	m_asyncExecutor.SpawnTask(asyncTask.get());
}

void ProcessingManager::RunNewTask(Task *task) {
	m_executor.UpdateTaskStatus(task->GetID(), TaskState::Running);
	TRACE_INFO("Task-%s: Running", task->GetID().c_str());
	if(task != nullptr) {
		try {
			task->Run();
			m_executor.UpdateTaskStatus(task->GetID(), TaskState::Finished);
		}
		catch (Exception &e) {
			m_executor.UpdateTaskStatus(task->GetID(), TaskState::Failed);
		}
	}
	OnTaskCompletion(task);
}

void ProcessingManager::OnTaskCompletion(Task* task)
{
	delete task;
	{
		unique_lock<mutex> lock(m_completionMut);
		auto it = m_runningAsyncTasks.begin();
		while(it != m_runningAsyncTasks.end()){
			if((*it)->GetState() == AsyncTask::AsyncTaskState::COMPLETED) {
				it = m_runningAsyncTasks.erase(it);
			}
			else
				it++;
		}
	}
}


