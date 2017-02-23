#include "Scheduler.h"
#include "Core/Logger.h"
#include <mesos/resources.hpp>

using namespace std;

void Scheduler::Initialize()
{
	for(const ConfigParams::NodePUMap::value_type& pair : ConfigParams::Instance().GetProcessingUnitsMap())
	{
		for(int id : pair.second)
			m_processingUnits[pair.first].PushBack(Pu(id));
	}
	for(const std::unordered_map<ConfigParams::NodeAddress, core::OrderedList<Pu>>::value_type& pair : m_processingUnits)
	{
		m_processingUnits[pair.first].Sort();
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
		mesos::Resources resources = offer.resources();
		list<Job> noneQualifiedJobs;
		list<pair<Job, pair<mesos::Resource, mesos::SlaveID>>> qualifiedJobs;
		Job job;
		while(m_jobsQueue.TryAndPop(job))
		{
			mesos::Resource taskResource = mesos::Resources::parse("cpus", job.coreCount, "*").get();
			if(resources.contains(taskResource) == true)
			{
				qualifiedJobs.push_back({job,{taskResource, offer.slave_id()}});
				resources -= taskResource;
				//Check to see if resources is empty, if it is its safe to stop the resource check, another condition
				//is if the resource is out of atleast one physical core.
				if(resources.empty() == true || resources.contains(mesos::Resources::parse("cpus", "1", "*").get()) == false)
					break;
			}
			else
				noneQualifiedJobs.push_back(job);
		}
		m_jobsQueue.Push(noneQualifiedJobs);
		vector<mesos::TaskInfo> tasks;
		BuildTasks(qualifiedJobs, tasks);
		driver->launchTasks(offer.id(), tasks);
	}
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
	string mesosMasterAddress = ConfigParams::Instance().GetMesosMasterAddress() + ":" + to_string(ConfigParams::Instance().GetMesosMasterPort());
	m_mesosDriver.reset(new mesos::MesosSchedulerDriver(this, frameWorkInfo, mesosMasterAddress));
	m_mesosDriver->start();
}


void Scheduler::BuildTasks(const list<pair<Job,pair<mesos::Resource, mesos::SlaveID>>>& taskedJobs, vector<mesos::TaskInfo>& tasks)
{
	for(const pair<Job, pair<mesos::Resource, mesos::SlaveID>>& taskedJob : taskedJobs)
	{
		tasks.push_back(mesos::TaskInfo());
		tasks.back().set_name(string("Task-") + to_string(taskedJob.first.id));
		tasks.back().mutable_task_id()->set_value(to_string(taskedJob.first.id));
		tasks.back().mutable_slave_id()->MergeFrom(taskedJob.second.second);
		tasks.back().mutable_executor()->MergeFrom(m_executorInfo);
		tasks.back().mutable_resources()->MergeFrom(mesos::Resources(taskedJob.second.first));
		
		m_activatedTasks.AddValue(taskedJob.first.id, Task());
	}	
}
