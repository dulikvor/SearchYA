#include "Task.h"
#include "Core/AsyncTask.h"
#include "Core/Logger.h"
#include "RedisSearchModule/Document.h"
#include "ProcessingManager.h"
#include "IndexBuilder.h"

using namespace std;
using namespace core;

Task::~Task()
{
	for(AsyncTask* activity : m_taskActivities)
		delete activity;
}

void Task::Run(std::function<void()> *const specificOnInit)
{
	TRACE_INFO("Task-%s started to run", m_id.c_str());
	AsyncTask* initTask = new AsyncTask(*specificOnInit);
	m_taskActivities.push_back(initTask);
	m_manager.SubmitTaskActivity(initTask);
}

void Task::OnInit(std::vector<std::function<void()>> *const specificOnProcessingList)
{
	TRACE_INFO("Task-%s OnInit", m_id.c_str());
	vector<AsyncTask*> tasksList;
	for(auto& onProcessingFunctor : *specificOnProcessingList)
	{
		tasksList.push_back(new AsyncTask(onProcessingFunctor));
		m_processingTaskCount++;
	}
	m_manager.SubmitProcessingActivities(tasksList, m_coreCount);
	m_taskActivities.insert(m_taskActivities.end(), tasksList.begin(), tasksList.end());
	
	m_state = TaskState::Running;
	IndexBuilder::Instance().GetExecutor().UpdateTaskStatus(m_id ,m_state);
}

void Task::OnProcessing(int subTaskID, std::function<void()> *const processingFunction)
{
	TRACE_INFO("Task-%s onProcessing sub task - %d", m_id.c_str(), subTaskID);
	try
	{
		(*processingFunction)();
	}
	catch(Exception& e)
	{
		m_failure = true;
		m_reason = string("Failure");
	}
	AsyncTask* finishTask = new AsyncTask(bind(&Task::OnFinished, this, subTaskID));
	m_taskActivities.push_back(finishTask);
	m_manager.SubmitTaskActivity(finishTask);
}

void Task::OnFinished(int subTaskID)
{
	TRACE_INFO("Task-%s OnFinished sub task - %d", m_id.c_str(), subTaskID);
	m_processingTaskCount--;
	if(m_processingTaskCount == 0) //all is finished
	{
		if(!m_failure) //all is well
			m_state = TaskState::Finished;
		else
			m_state = TaskState::Failed;

		IndexBuilder::Instance().GetExecutor().UpdateTaskStatus(m_id ,m_state);
		m_manager.SubmitTaskCompletion(m_id);
	}
}
