#include "Executor.h"
#include <string>
#include <mesos/mesos.pb.h>
#include "Core/src/Logger.h"
#include "Communication/GeneralParams.h"
#include "Communication/TaskMetaData.h"
#include "Communication/Serializor.h"
#include "cppkin/cppkin.h"
#include "IndexBuilder.h"
#include "TasksFactoryContainer.h"

using namespace std;
using namespace core;

Executor::~Executor()
{
	m_driver->abort();
}

void Executor::Init()
{
	m_driver.reset(new mesos::MesosExecutorDriver(this));
	m_driver->start();
}

void Executor::registered(mesos::ExecutorDriver* driver, 
		const mesos::ExecutorInfo& executorInfo, const mesos::FrameworkInfo& frameworkInfo, 
		const mesos::SlaveInfo& slaveInfo)
{
	TRACE_INFO("Registered with - FW %s, on Slave(host:id) %s:%s", 
			frameworkInfo.name().c_str(), slaveInfo.hostname().c_str(), 
			slaveInfo.id().value().c_str());
}

void Executor::reregistered(mesos::ExecutorDriver* driver, 
		const mesos::SlaveInfo& slaveInfo)
{
	TRACE_INFO("Reregistered with - Slave(host:id) %s:%s", slaveInfo.hostname().c_str(), 
			slaveInfo.id().value().c_str());
}

void Executor::disconnected(mesos::ExecutorDriver* driver)
{
	TRACE_INFO("Disconnected from mesos slave");
	IndexBuilder::Instance().Terminate();
}
//
void Executor::launchTask(mesos::ExecutorDriver* driver, const mesos::TaskInfo& task)
{
	TRACE_INFO("Task received %s label - %s", task.task_id().value().c_str(), task.labels().labels(0).value().c_str());

	Serializor context(task.data().c_str(), task.data().size());
	TaskMetaData taskMetaData = TaskMetaData::Deserialize(context);

    cppkin::Span::SpanHeader spanHeader;
    DESERIALIZE_TRACING_HEADER(Thrift, taskMetaData.TracingData.first.get(), taskMetaData.TracingData.second, spanHeader);

	CREATE_SPAN((string("Processing task - ") + task.task_id().value()).c_str(), spanHeader.TraceID, spanHeader.ID);

	double coresRequired = 0;
	for(int index = 0; index < task.resources_size(); index++)
	{
		const mesos::Resource& resource = task.resources(index);
		if(resource.name() == "cpus")
			coresRequired += resource.scalar().value();
	}
	string taskType = GetLabelValue("Task Type", task.labels());
	IndexBuilder::Instance().GetProcessingManager().SubmitNewTask(TaskType::FromString(taskType),
	task.task_id().value(), coresRequired, taskMetaData.TaskData.first.get(), taskMetaData.TaskData.second);
	TRACE_EVENT((string("Task - ") + task.task_id().value() + " running").c_str());
	SUBMIT_SPAN();
}
//
void Executor::killTask(mesos::ExecutorDriver* driver, const mesos::TaskID& taskId)
{
}
//
void Executor::frameworkMessage(mesos::ExecutorDriver* driver, const std::string& data)
{
}
//
void Executor::shutdown(mesos::ExecutorDriver* driver)
{
	TRACE_INFO("Shutdown was received");
	IndexBuilder::Instance().Terminate();
}
//
void Executor::error(mesos::ExecutorDriver* driver, const std::string& message)
{
}

void Executor::UpdateTaskStatus(const std::string& taskID, TaskState state)
{
	mesos::TaskStatus status;
	status.mutable_task_id()->set_value(taskID);
	status.set_state(static_cast<mesos::TaskState>((int)state));
	m_driver->sendStatusUpdate(status);
}

void Executor::SendMessage(const std::string& message)
{
	m_driver->sendFrameworkMessage(message);
}

string Executor::GetLabelValue(const string& labelName, const mesos::Labels& labels){
	for(int index = 0; index < labels.labels_size(); index++){
		const mesos::Label& label = labels.labels(index);
		if(label.key() == labelName)
			return label.value();
	}
	throw Exception(SOURCE, "Requested label wasn't received - %s", labelName.c_str());
}
