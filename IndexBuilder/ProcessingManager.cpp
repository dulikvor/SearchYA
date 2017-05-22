#include "ProcessingManager.h"
#include "Core/AsyncTask.h"
#include "Core/Enviorment.h"
#include "Communication/GeneralParams.h"
#include "IndexingTask.h"

using namespace std;
using namespace core;

ProcessingManager::ProcessingManager()
	:m_taskCompletion(nullptr)
{
	m_computationAsyncExecutor.reset(new AsyncExecutor(
				Enviorment::Instance().GetCoreCount(), false));
}

ProcessingManager::~ProcessingManager()
{
	delete m_taskCompletion;
}

Task* ProcessingManager::GetTask(const TaskID& taskID)
{
	if(m_tasks.ContainsKey(taskID) == false)
		throw Exception(SOURCE, "Requested task dosn't exits - %s", taskID.c_str());
	return m_tasks[taskID];
}

void ProcessingManager::SubmitNewTask(const string& taskID, int coreCount)
{
	if(m_tasks.ContainsKey(taskID) == true)
		throw Exception(SOURCE, "Received task already exists - %s", taskID.c_str());
	
	Task* task = new IndexingTask(*this, taskID, coreCount);
	m_tasks[taskID] = task;
	task->Run();
}

void ProcessingManager::SubmitTaskActivity(AsyncTask* activity)
{
	m_executor.SpawnTask(activity);
}

void ProcessingManager::SubmitProcessingActivities(vector<AsyncTask*> activities, int coreCount)
{
	while(coreCount-- > 0)
		m_computationAsyncExecutor->IncActiveThreads();
	for(AsyncTask* activity : activities)
		m_computationAsyncExecutor->SpawnTask(activity);
}

void ProcessingManager::SubmitTaskCompletion(const string& taskID)
{
	unique_lock<mutex> lock(m_completionMut);
	AsyncTask* task = new AsyncTask(bind(&ProcessingManager::OnTaskCompletion, this, taskID));
	m_executor.SpawnTask(task);
	m_completionAsyncTasks.push_back(task);
}

void ProcessingManager::OnTaskCompletion(const string taskID)
{
	if(m_tasks.ContainsKey(taskID) == false)
		throw Exception(SOURCE, "Provided key dosn't exits - %s", taskID.c_str());
	Task* task = m_tasks[taskID];
	m_tasks.RemoveValue(taskID);
	for(int index = 0; index < task->GetCoreCount(); index++)
		m_computationAsyncExecutor->DecActiveThreads();
	delete task;
	delete m_taskCompletion;
	{
		unique_lock<mutex> lock(m_completionMut);
		m_taskCompletion = m_completionAsyncTasks.front();
		m_completionAsyncTasks.pop_front();
	}
}


