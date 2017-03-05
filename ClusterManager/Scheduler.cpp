#include "Scheduler.h"
#include <mesos/resources.hpp>
#include "Core/Logger.h"
#include "TaskState.h"

using namespace std;

void Scheduler::Initialize()
{
	for(const ConfigParams::NodePUMap::value_type& pair : 
			ConfigParams::Instance().GetProcessingUnitsMap())
	{
		for(int id : pair.second)
			m_processingUnits[pair.first].PushBack(Pu(id));
	}
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
		list<Job> noneQualifiedJobs;
		list<pair<Job, pair<mesos::Resources, mesos::SlaveID>>> qualifiedJobs;
		Job job;
		{
			lock_guard<mutex> lock(m_jobsMut);	
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
		vector<mesos::TaskInfo> tasks;
		BuildTasks(qualifiedJobs, tasks);
		driver->launchTasks(offer.id(), tasks);
	}
}

void Scheduler::statusUpdate(mesos::SchedulerDriver* driver, const mesos::TaskStatus& status)
{
	TRACE_INFO("Task - %s status - %d", status.task_id().value().c_str(), (int)status.state());
	if(status.state() == mesos::TaskState::TASK_FINISHED)
		m_activatedTasks.RemoveValue(status.task_id().value());
}
void Scheduler::AddJob(const Job& job)
{
	lock_guard<mutex> lock(m_jobsMut);	
	m_jobsQueue.Push(job); 
}

void Scheduler::InitializeMesos()
{
	m_executorInfo.mutable_executor_id()->set_value("Executor");
	m_executorInfo.mutable_command()->set_value(ConfigParams::Instance().GetExecDir() + "/Executor");
	m_executorInfo.set_name("Stub Executor");
	mesos::DiscoveryInfo* discoveryInfo = m_executorInfo.mutable_discovery();
	discoveryInfo->set_visibility(mesos::DiscoveryInfo_Visibility::DiscoveryInfo_Visibility_FRAMEWORK);
	discoveryInfo->set_version("0.1");
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
		//tasks.back().mutable_executor()->MergeFrom(m_executorInfo);
		mesos::CommandInfo command;
		command.set_shell(true);
		command.set_value("echo 'Hello World'");
		tasks.back().mutable_command()->MergeFrom(command);
		tasks.back().mutable_resources()->MergeFrom(mesos::Resources(taskedJob.second.first));
		
		m_activatedTasks.AddValue(taskID, Task());
	}	
}
