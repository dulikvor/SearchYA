#include "Task.h"
#include <functional>
#include "Core/AsyncTask.h"
#include "Core/Logger.h"
#include "ProcessingManager.h"
#include "IndexBuilder.h"

using namespace std;
using namespace core;

Task::~Task()
{
	for(AsyncTask* activity : m_taskActivities)
		delete activity;
}

void Task::Run()
{
	TRACE_INFO("Task run begin %s", m_id.c_str());
	AsyncTask* initTask = new AsyncTask(bind(&Task::OnInit, this));
	m_taskActivities.push_back(initTask);
	m_manager.SubmitTaskActivity(initTask);
}

void Task::OnInit()
{
	TRACE_INFO("Task-%s init task", m_id.c_str());
	//Do something to init
	list<AsyncTask*> tasksList;
	for(int index  = 0; index < 2; index++)
	{	
		AsyncTask* task = new AsyncTask(bind(&Task::OnProcessing, this, index));
		tasksList.push_back(task);
		m_processingTaskCount++;
	}
	m_manager.SubmitProcessingActivities(tasksList, m_coreCount);
	m_taskActivities.splice(m_taskActivities.end(), tasksList);
	
	m_state = TaskState::Running;
	IndexBuilder::Instance().GetExecutor().UpdateTaskStatus(m_id ,m_state);
}

void Task::OnProcessing(int id)
{
	TRACE_INFO("Task-%s processing task %d", m_id.c_str(), id);
	try
	{
	}
	catch(Exception& e)
	{
		m_failure = true;
		m_reason = string("Failure");
	}
	AsyncTask* finishTask = new AsyncTask(bind(&Task::OnFinished, this, id));
	m_taskActivities.push_back(finishTask);
	m_manager.SubmitTaskActivity(finishTask);
}

void Task::OnFinished(int id)
{
	TRACE_INFO("Task-%s processing finished %d", m_id.c_str(), id);
	m_processingTaskCount--;
	if(m_processingTaskCount == 0) //all is finished
	{
		if(m_failure == false) //all is well
			m_state = TaskState::Finished;
		else
			m_state = TaskState::Failed;

		IndexBuilder::Instance().GetExecutor().UpdateTaskStatus(m_id ,m_state);
		m_manager.SubmitTaskCompletion(m_id);
	}
}
