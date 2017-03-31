#include "Scheduler.h"
#include <atomic>
#include <iterator>
#include <mesos/resources.hpp>
#include "Core/Logger.h"
#include "Communication/GeneralParams.h"
#include "TaskState.h"
#include "ClusterManager.h"

using namespace std;

Scheduler::Scheduler(const std::string& role):m_role(role)
{
}

void Scheduler::Initialize()
{
	InitializeMesos();
}


void Scheduler::registered(mesos::SchedulerDriver* driver, const mesos::FrameworkID& frameworkId, const mesos::MasterInfo& masterInfo)
{
	TRACE_INFO("Scheduler is connected to master, fwid - %s, scheduler host -%s, master host - %s:%d", frameworkId.value().c_str(), ConfigParams::Instance().GetHostAddress().c_str(), masterInfo.address().ip().c_str(), masterInfo.address().port());
}

void Scheduler::resourceOffers(mesos::SchedulerDriver* driver, const vector<mesos::Offer>& offers)
{

	for(const mesos::Offer& offer : offers)
	{

		for(const mesos::Resource& resource : offer.resources())
		{
			TRACE_VERBOSE("Offer - %s Resource: Name - %s Type - %d Count - %f, Role - %s", 
					offer.id().value().c_str() ,resource.name().c_str(), resource.type(), 
					resource.scalar().value(), resource.role().c_str());
		}

		mesos::Resources resources = offer.resources();
		vector<mesos::TaskInfo> tasks;
		list<pair<Job, pair<mesos::Resources, mesos::SlaveID>>> qualifiedJobs;
		list<pair<mesos::Resources, mesos::SlaveID>> wakeUpJobs;
		if(ISExecutorValid(offer.slave_id().value(), "Executor"))
		{
			list<Job> noneQualifiedJobs;
			Job job;
			{
				lock_guard<mutex> lock(m_mutex);	
				while(m_jobsQueue.TryAndPop(job))
				{
					mesos::Resources taskResources = mesos::Resources::parse(string("cpus:") + 
							to_string(job.coresCount) + ";").get();
					taskResources.allocate(m_role);
					for(const mesos::Resource& resource : taskResources.flatten())
					{
						TRACE_VERBOSE("Task Resource - ID - %d Name - %s Type - %d Count - %f Role - %s", (int)job.id, resource.name().c_str(), resource.type(), 
								resource.scalar().value(), resource.role().c_str());
					}
					if(resources.flatten().contains(taskResources) == true)
					{
						qualifiedJobs.push_back({job,{taskResources, offer.slave_id()}});
						resources -= taskResources;
					}
					else
						noneQualifiedJobs.push_back(job);
				}
				m_jobsQueue.Push(noneQualifiedJobs);
			}
		}
		else
		{
			mesos::Resources taskResources = mesos::Resources::parse(string("cpus:") + 
					to_string((float)0.1) + ";").get();
			taskResources.allocate(m_role);
			if(resources.flatten().contains(taskResources) == true)
			{
				wakeUpJobs.push_back({taskResources, offer.slave_id()});
				resources -= taskResources;
			}
		}
		BuildWakeUpTasks(wakeUpJobs, tasks);
		BuildTasks(qualifiedJobs, tasks);
		driver->launchTasks(offer.id(), tasks);
	}
}

void Scheduler::frameworkMessage(mesos::SchedulerDriver* driver, 
		const mesos::ExecutorID& executorId, const mesos::SlaveID& slaveId, 
		const std::string& data)
{

	ClusterManager::Instace().HandleMesosMessage({slaveId.value(), executorId.value()}, data);
}

void Scheduler::statusUpdate(mesos::SchedulerDriver* driver, const mesos::TaskStatus& status)
{
	TRACE_INFO("Task - %s status - %d", status.task_id().value().c_str(), (int)status.state());
	if(status.state() == mesos::TaskState::TASK_FINISHED)
		m_activatedTasks.RemoveValue(status.task_id().value());
}
void Scheduler::AddJob(const Job& job)
{
	lock_guard<mutex> lock(m_mutex);	
	m_jobsQueue.Push(job); 
}

void Scheduler::InitializeMesos()
{
	m_executorInfo.mutable_executor_id()->set_value("Executor");
	m_executorInfo.mutable_command()->set_value(ConfigParams::Instance().GetExecDir() + 
			"/IndexBuilder");
	m_executorInfo.mutable_command()->set_shell(false);
	m_executorInfo.set_name("Stub Executor");
	//mesos::DiscoveryInfo* discoveryInfo = m_executorInfo.mutable_discovery();
	//discoveryInfo->set_visibility(mesos::DiscoveryInfo_Visibility::DiscoveryInfo_Visibility_FRAMEWORK);
	//discoveryInfo->set_version("0.1");
	//Labels missing from executor info???
	//appc::spec::ImageManifest_Label* label = m_executorInfo.mutable_labels()->Add();
	//Add a label which depicts where the ClusterManager will try to resolve the GRPC communication.
	//label->set_name("GRPC:Host");
	//label->set_value(ConfigParams::Instance().GetHostAddress() + ":300");
	mesos::FrameworkInfo frameWorkInfo;
	frameWorkInfo.set_user("");
	frameWorkInfo.set_name("Stub FrameWork");
	frameWorkInfo.set_failover_timeout(ConfigParams::Instance().GetRecoveryTime());
	frameWorkInfo.set_hostname(ConfigParams::Instance().GetHostAddress());
	string mesosMasterAddress = ConfigParams::Instance().GetMesosMasterAddress() + ":5050";
	m_mesosDriver.reset(new mesos::MesosSchedulerDriver(this, frameWorkInfo, mesosMasterAddress));
	m_mesosDriver->start();
}


void Scheduler::BuildTasks(const list<pair<Job,pair<mesos::Resources, mesos::SlaveID>>>& taskedJobs, vector<mesos::TaskInfo>& tasks)
{
	for(const pair<Job, pair<mesos::Resources, mesos::SlaveID>>& taskedJob : taskedJobs)
	{
		TRACE_INFO("Task %d to be submmited at - %s", taskedJob.first.id, taskedJob.second.second.value().c_str()); 
		tasks.push_back(mesos::TaskInfo());
		string taskID = string("Task-") + to_string(taskedJob.first.id);
		tasks.back().set_name(taskID);
		tasks.back().mutable_task_id()->set_value(to_string(taskedJob.first.id));
		tasks.back().mutable_slave_id()->MergeFrom(taskedJob.second.second);
		tasks.back().mutable_executor()->MergeFrom(m_executorInfo);
		mesos::Label* label = tasks.back().mutable_labels()->add_labels();
		label->set_key("Task Type");
		label->set_value("Processing");
		//mesos::CommandInfo command;
		//command.set_shell(true);
		//command.set_value("echo 'Hello World'");
		//tasks.back().mutable_command()->MergeFrom(command);
		tasks.back().mutable_resources()->MergeFrom(mesos::Resources(taskedJob.second.first));
		
		m_activatedTasks.AddValue(taskID, Task());
	}	
}

void Scheduler::AddExecutor(const std::string& slaveID, const std::string& executorID)
{
	lock_guard<mutex> lock(m_mutex);	
	pair<string, string> key = {slaveID, executorID};
	if(m_executors.ContainsKey(key) == false)
	{
		m_executors.AddValue(key, Executor());
	}
}


void Scheduler::BuildWakeUpTasks(std::list<std::pair<mesos::Resources, mesos::SlaveID>>& locations, std::vector<mesos::TaskInfo>& tasks)
{
	GeneralParams params;
	params.AddParam("Redis Server Address", ConfigParams::Instance().GetRedisServerAddress());
	for(const pair<mesos::Resources, mesos::SlaveID>& location : locations)
	{
		static int taskID = 0;
		TRACE_INFO("Wakeup Task to be submmited at - %s Exec - %s", location.second.value().c_str(), m_executorInfo.command().value().c_str()); 
		tasks.push_back(mesos::TaskInfo());
		tasks.back().set_name(string("WATask-") + location.second.value());
		tasks.back().mutable_task_id()->set_value("WA" + to_string(taskID++));
		tasks.back().mutable_slave_id()->MergeFrom(location.second);
		tasks.back().mutable_executor()->MergeFrom(m_executorInfo);
		mesos::Label* label = tasks.back().mutable_labels()->add_labels();
		label->set_key("Task Type");
		label->set_value("Wake Up");
		Serializor serializor;
		params.Serialize(serializor);
		tasks.back().set_data(serializor.GetBuffer());
		//mesos::CommandInfo command;
		//command.set_shell(true);
		//command.set_value("echo 'Hello World'");
		//tasks.back().mutable_command()->MergeFrom(command);
		tasks.back().mutable_resources()->MergeFrom(location.first);
	}	
}

bool Scheduler::ISExecutorValid(const std::string& slaveID, const std::string& executorID)
{
	return m_executors.ContainsKey({slaveID, executorID});
}
