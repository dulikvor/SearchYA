#include "ProcessingManager.h"
#include "Core/AsyncTask.h"
#include "Core/Enviorment.h"
#include "Communication/GeneralParams.h"
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

	AsyncTask* task = new AsyncTask(bind(&ProcessingManager::RunNewTask, this, executorTask.release()));
	{
		unique_lock<mutex> lock(m_completionMut);
		m_completedAsyncTasks.push_back(task);
	}
	m_asyncExecutor.SpawnTask(task);
}

void ProcessingManager::RunNewTask(Task *task) {
	m_executor.UpdateTaskStatus(task->GetID(), TaskState::Running);
	TRACE_INFO("Task-%s: Running", task->GetID().c_str());
	if(task != nullptr) {
		try {
			task->Run();
		}
		catch (Exception &e) {
			m_executor.UpdateTaskStatus(task->GetID(), TaskState::Failed);
		}
	}
	m_executor.UpdateTaskStatus(task->GetID(), TaskState::Finished);
	OnTaskCompletion(task);
}

void ProcessingManager::OnTaskCompletion(Task* task)
{
	delete task;
	{
		unique_lock<mutex> lock(m_completionMut);
		auto it = m_completedAsyncTasks.begin();
		while(it != m_completedAsyncTasks.end()){
			if((*it)->GetState() == AsyncTask::AsyncTaskState::COMPLETED) {
				delete *it;
				it = m_completedAsyncTasks.erase(it);
			}
			else
				it++;
		}
	}
}


