#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <utility>
#include <memory>
#include <mesos/scheduler.hpp>
#include <mesos/mesos.pb.h>
#include "Core/SyncQueue.h"
#include "Core/OrderedList.h"
#include "Core/ConcurrentDictionary.h"
#include "ConfigParams.h"
#include "Pu.h"
#include "Job.h"
#include "Task.h"

class Task;

class Scheduler: public mesos::Scheduler
{
public:
	Scheduler(){}
	~Scheduler(){}
	//Initialize will address the config data reading the cluster information, the different processing units locations
	//indicated by thier node address (they addresses may return). the function will create an internal object representing
	//each processing unit, storing each process by its location address, no need to sort the most busy processes, since they are all idle
	//at the moment.
	void Initialize();
	//Mesos Scheduler API overrides
	//registered implements the mesos scheduler "registered" routine, the function will print the address of mesos master and the
	//given cluster manager address upon registration.
	void registered(mesos::SchedulerDriver* driver, const mesos::FrameworkID& frameworkId, const mesos::MasterInfo& masterInfo);
	//resourceOffers implements mesos scheduler "resourceOffers" interface, the function will do the following:
	//1) Itarting across all received resources.
	//2) for each offered resources instance it will inspect its jobs queue, if the given job qualifies for becoming a task 
	//	it will be stored and removed from the jobs queue.
	//3) All qualified jobs are transformed to tasks.
	void reregistered(mesos::SchedulerDriver*, const mesos::MasterInfo& masterInfo) override  {}
	void resourceOffers(mesos::SchedulerDriver* driver, const std::vector<mesos::Offer>& offers) override;
	void disconnected(mesos::SchedulerDriver* driver) override {}
	void offerRescinded(mesos::SchedulerDriver* driver, const mesos::OfferID& offerId) override{}
	void statusUpdate(mesos::SchedulerDriver* driver, const mesos::TaskStatus& status) override {}
	void frameworkMessage(mesos::SchedulerDriver* driver, const mesos::ExecutorID& executorId, const mesos::SlaveID& slaveId, const std::string& data) override {}
	void slaveLost(mesos::SchedulerDriver* driver, const mesos::SlaveID& sid) override {}
	void executorLost(mesos::SchedulerDriver* driver,const mesos::ExecutorID& executorID,const mesos::SlaveID& slaveID,int status) override {}
	void error(mesos::SchedulerDriver* driver, const std::string& message) override {}
	//
	//AddJob adds a new job to the jobs queue.
	void AddJob(const Job& job){ m_jobsQueue.Push(job); }

private:
	void InitializeMesos();
	//
	void BuildTasks(const std::list<std::pair<Job,std::pair<mesos::Resource, mesos::SlaveID>>>& jobs,
			std::vector<mesos::TaskInfo>& tasks);

private:
	using TaskID = int;
private:
	std::unordered_map<ConfigParams::NodeAddress, Core::OrderedList<Pu>> m_processingUnits;
	std::unique_ptr<mesos::SchedulerDriver> m_mesosDriver;
	Core::ConcurrentDictionary<TaskID, Task> m_activatedTasks; 
	Core::SyncQueue<Job> m_jobsQueue;
	mesos::FrameworkInfo m_frameWorkInfo;
	mesos::ExecutorInfo m_executorInfo;
};
