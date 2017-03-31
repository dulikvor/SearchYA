#include "Executor.h"
#include <mesos/mesos.pb.h>
#include "Core/Logger.h"
#include "Communication/GeneralParams.h"
#include "Communication/Serializor.h"
#include "IndexBuilder.h"

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
	IndexBuilder::Instance().NewCommand(CommandType::Terminate, GeneralParams());
}
//
void Executor::launchTask(mesos::ExecutorDriver* driver, const mesos::TaskInfo& task)
{
	TRACE_INFO("Task received %s label - %s", task.task_id().value().c_str(), task.labels().labels(0).value().c_str()); 
	double coresRequired = 0;
	for(int index = 0; index < task.resources_size(); index++)
	{
		const mesos::Resource& resource = task.resources(index);
		if(resource.name() == "cpus")
			coresRequired += resource.scalar().value();
	}
	GeneralParams params;
	if(task.data().size() > 0)
	{
		Serializor serializor(task.data().data(), task.data().size());
		params.Deserialize(serializor);
	}
	params.AddParam("Core Count", (int)(coresRequired) + 1);
	params.AddParam("Task ID", task.task_id().value());
	const mesos::Label& label = task.labels().labels(0);
	if(label.key() == "Task Type" && label.value() == "Processing")
		IndexBuilder::Instance().NewCommand(CommandType::Process, params);
	else //Init task
	{
		IndexBuilder::Instance().NewCommand(CommandType::Init, params);
		SendInitAck(params);
	}
}
//
void Executor::killTask(mesos::ExecutorDriver* driver, const mesos::TaskID& taskId)
{
}
//
void Executor::frameworkMessage(mesos::ExecutorDriver* driver, const std::string& data)
{
//	AsyncTask task(bind(&IndexBuilder::OnNewMessage, this));
//	m_asyncExecutor.SpawnTask(&task);
//	task.Wait();
}
//
void Executor::shutdown(mesos::ExecutorDriver* driver)
{
	TRACE_INFO("Shutdown was received");
	IndexBuilder::Instance().NewCommand(CommandType::Terminate, GeneralParams());
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

void Executor::SendInitAck(const GeneralParams& params)
{
	Serializor serializor;
	Serializor::Serialize(serializor, -1, (int)MessageType::InitAck);
	SendMessage(serializor.GetBuffer());
	UpdateTaskStatus(StringConverter::Convert(params.GetValue("Task ID")), TaskState::Finished);
}
