#include "Scheduler.h"
#include "Core/Logger.h"
#include "Core/CommandLine.h"
#include "Communication/GeneralParams.h"
#include "ClusterManager.h"
#include "JobFactoryContainer.h"
#include "TaskState.h"
#include "GetTopKJob.h"
#include "GetTopKTask.h"

using namespace std;

Scheduler::Scheduler(const std::string& role):m_role(role)
{
}

Scheduler::~Scheduler() {
	vector<TaskID> tasksIDs = m_activatedTasks.GetAllKeys();
	for(const TaskID& taskID : tasksIDs)
		delete m_activatedTasks[taskID];
}

void Scheduler::Initialize()
{
	InitializeMesos();
}


void Scheduler::registered(mesos::SchedulerDriver* driver, const mesos::FrameworkID& frameworkId, const mesos::MasterInfo& masterInfo)
{
	TRACE_INFO("Scheduler is connected to master, fwid - %s, scheduler host -%s, master host - %s:%d", frameworkId.value().c_str(), ConfigParams::Instance().GetHostAddress().c_str(), masterInfo.address().ip().c_str(), masterInfo.address().port());
}

void Scheduler::AllocateQualifiedProcessingJobsResources(mesos::Resources& offeredResources,
                const mesos::SlaveID& slaveID, vector<JobAllocatedResources>& allocatedJobs)
{
	vector<Job*> noneQualifiedJobs;
	Job* job;
	{
		lock_guard<mutex> lock(m_mutex);
		while(m_jobsQueue.TryAndPop(job))
		{
			mesos::Resources taskResources = mesos::Resources::parse(string("cpus:") +
                 to_string(job->GetRequiredCores()) + ";", m_role).get();
			if(offeredResources.flatten().contains(taskResources))
			{
				allocatedJobs.push_back({unique_ptr<Job>(job),{taskResources, slaveID}});
				offeredResources -= taskResources;
			}
			else
				noneQualifiedJobs.push_back(job);
		}
		m_jobsQueue.Push(noneQualifiedJobs);
	}
}

void Scheduler::AllocateInitJobResources(mesos::Resources &offeredResources,
          const mesos::SlaveID &slaveID, vector<JobAllocatedResources>& allocatedJobs)
{
	static atomic_int id(0);
	unique_ptr<Job> job = JobFactoryContainer::Instance().Create(JobType::Init, id++);
	static mesos::Resources taskResources = mesos::Resources::parse(string("cpus:") +
         to_string(job->GetRequiredCores()) + ";", m_role).get();
	if(offeredResources.flatten().contains(taskResources))
		offeredResources -= taskResources;

	allocatedJobs.emplace_back(make_pair(move(job), make_pair(taskResources, slaveID)));
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
		vector<JobAllocatedResources> qualifiedJobs;
		if(ISExecutorValid(offer.slave_id().value(), "Executor"))
			AllocateQualifiedProcessingJobsResources(resources, offer.slave_id(), qualifiedJobs);
		else
		    AllocateInitJobResources(resources, offer.slave_id(), qualifiedJobs);

		vector<mesos::TaskInfo> tasks;
		BuildTasks(qualifiedJobs, tasks);
		driver->launchTasks(offer.id(), tasks);
	}
}

void Scheduler::frameworkMessage(mesos::SchedulerDriver* driver, 
		const mesos::ExecutorID& executorId, const mesos::SlaveID& slaveId, 
		const std::string& data)
{
    TRACE_INFO("FrameWork Message received - ExecutorID - %s, SlaveID - %s", executorId.value().c_str(),
        slaveId.value().c_str());

}

void Scheduler::statusUpdate(mesos::SchedulerDriver* driver, const mesos::TaskStatus& status)
{
	TRACE_INFO("Task - %s status - %s", status.task_id().value().c_str(),
			   TaskState(TaskState::Enumeration(status.state())).ToString().c_str());
	if(status.state() == mesos::TaskState::TASK_FINISHED)
		m_activatedTasks.RemoveValue(status.task_id().value());
}
void Scheduler::AddJob(unique_ptr<Job> job)
{
	lock_guard<mutex> lock(m_mutex);	
	m_jobsQueue.Push(job.release());
}

void Scheduler::InitializeMesos()
{
	m_executorInfo.mutable_executor_id()->set_value("Executor");
    m_executorInfo.set_type(mesos::ExecutorInfo::CUSTOM);
	m_executorInfo.mutable_command()->set_value(ConfigParams::Instance().GetExecDir() + 
			"/IndexBuilder");
	m_executorInfo.mutable_command()->set_shell(false);
	mesos::Environment_Variable* envVar = m_executorInfo.mutable_command()->mutable_environment()->add_variables();
	envVar->set_name("LD_LIBRARY_PATH");
	envVar->set_value(ConfigParams::Instance().GetExecDir() + "/../Third_Party/lib");
	m_executorInfo.mutable_command()->mutable_arguments()->Add(
			string("-workingdir=") + CommandLine::Instance().GetArgument("workingdir"));

	m_executorInfo.set_name("Stub Executor");
	mesos::FrameworkInfo frameWorkInfo;
	frameWorkInfo.set_user("");
	frameWorkInfo.set_name("Stub FrameWork");
	frameWorkInfo.set_failover_timeout(ConfigParams::Instance().GetRecoveryTime());
	frameWorkInfo.set_hostname(ConfigParams::Instance().GetHostAddress());
	string mesosMasterAddress = ConfigParams::Instance().GetMesosMasterAddress() + ":5050";
	m_mesosDriver.reset(new mesos::MesosSchedulerDriver(this, frameWorkInfo, mesosMasterAddress));
	m_mesosDriver->start();
}


void Scheduler::BuildTasks(const vector<JobAllocatedResources> &jobsAndResources,
                           vector<mesos::TaskInfo> &tasks)
{
	for(const JobAllocatedResources & jobAllocatedResources : jobsAndResources)
	{
		const Job& job = *jobAllocatedResources.first;
		auto& resourceSlavePair = jobAllocatedResources.second;
		TRACE_INFO("Task %d to be submmited at - %s", job.GetID(),
                   resourceSlavePair.second.value().c_str());
		tasks.push_back(mesos::TaskInfo());
		string taskID = string(job.GetLabel()) + to_string(job.GetID());
		tasks.back().set_name(taskID);
		tasks.back().mutable_task_id()->set_value(job.GetLabel() + to_string(job.GetID()));
		tasks.back().mutable_slave_id()->MergeFrom(resourceSlavePair.second);
		tasks.back().mutable_executor()->MergeFrom(m_executorInfo);
		mesos::Label* label = tasks.back().mutable_labels()->add_labels();
		label->set_key("Task Type");
		label->set_value(job.GetLabel());
		tasks.back().mutable_resources()->MergeFrom(mesos::Resources(resourceSlavePair.first));
		unique_ptr<pair<const char*, int>, Job::Deleter> data = job.GenerateTaskData();
		if(data->second > 0)
			tasks.back().set_data(data->first, data->second);

		AddExecutor(resourceSlavePair.second.value(), "Executor"); //only one executor at the moment per slave
	}	
}

void Scheduler::AddExecutor(const std::string& slaveID, const std::string& executorID)
{
	lock_guard<mutex> lock(m_mutex);
	pair<string, string> key = {slaveID, executorID};
	if(!m_executors.ContainsKey(key))
		m_executors.AddValue(key, Executor());
}

void Scheduler::AddTask(const TaskID& taskID, const Job &job) {
	if(job.IsAsyncJob()){
		const GetTopKJob& typedJob = static_cast<const GetTopKJob&>(job);
		m_activatedTasks.AddValue(taskID, new GetTopKTask(typedJob.GetTag()));
	}
}

bool Scheduler::ISExecutorValid(const std::string& slaveID, const std::string& executorID)
{
	return m_executors.ContainsKey({slaveID, executorID});
}
