#pragma once

#include <string>
#include <utility>
#include <memory>
#include <mesos/scheduler.hpp>
#include <mesos/resources.hpp>
#include <mutex>
#include "Core/SyncQueue.h"
#include "Core/OrderedList.h"
#include "Core/ConcurrentDictionary.h"
#include "Core/TimedAsyncExecutor.h"
#include "Communication/MessageType.h"
#include "ConfigParams.h"
#include "Pu.h"
#include "Job.h"
#include "Task.h"
#include "Executor.h"

class GeneralParams;

class Task;
//Scheduler provides the following capabilities:
//1) Receiving incoming jobs.
//2) Implementing mesos scheduler api.
//3) Upon receiving resources from mesos - will try to tranformed stored jobs into tasks.
//4) Keeps bookeeping upon on going tasks.
class Scheduler: public mesos::Scheduler
{
public:
	Scheduler(const std::string& role);
	virtual ~Scheduler(){}
	//Initialize will address the config data reading the cluster information, the different processing units locations
	//indicated by thier node address (they addresses may return). the function will create an internal object representing
	//each processing unit, storing each process by its location address, no need to sort the most busy processes, since they are all idle
	//at the moment.
	void Initialize();
	//Mesos Scheduler API overrides
	//registered implements the mesos scheduler "registered" routine, the function will print the address of mesos master 
	//and the given cluster manager address upon registration.
	void registered(mesos::SchedulerDriver* driver, const mesos::FrameworkID& frameworkId, const mesos::MasterInfo& masterInfo) override;
	void reregistered(mesos::SchedulerDriver*, const mesos::MasterInfo& masterInfo) override  {}
	//resourceOffers implements mesos scheduler "resourceOffers" interface, the function will do the following:
	//1) Itarting across all received resources.
	//2) for each offered resources instance it will inspect its jobs queue, if the given job qualifies for becoming a task 
	//	(there are enough resources to satisfy the job requierments) will be stored and removed from the jobs queue.
	//3) All qualified jobs are transformed to tasks and launched.
	void resourceOffers(mesos::SchedulerDriver* driver, const std::vector<mesos::Offer>& offers) override;
	void disconnected(mesos::SchedulerDriver* driver) override {}
	void offerRescinded(mesos::SchedulerDriver* driver, const mesos::OfferID& offerId) override{}
	void statusUpdate(mesos::SchedulerDriver* driver, const mesos::TaskStatus& status) override;
	void frameworkMessage(mesos::SchedulerDriver* driver, const mesos::ExecutorID& executorId, const mesos::SlaveID& slaveId, const std::string& data) override;
	void slaveLost(mesos::SchedulerDriver* driver, const mesos::SlaveID& sid) override {}
	void executorLost(mesos::SchedulerDriver* driver,const mesos::ExecutorID& executorID,const mesos::SlaveID& slaveID,int status) override {}
	void error(mesos::SchedulerDriver* driver, const std::string& message) override {}
	//AddJob adds a new job to the jobs queue.
	void AddJob(const Job& job);
	//
	void SendMessage(std::string& data){}
	//
	void AddExecutor(const std::string& slaveID, const std::string& executorID);

public:
	using TaskID = std::string;
	using ExecutorID = std::string;
	using SlaveID = std::string;

	struct MessageSource
	{
		std::string SlaveID;
		std::string ExecutorID;
	};

private:
	//Will initiate mesos driver and connect to the master.
	void InitializeMesos();
	//Transforms received jobs into mesos tasks.
	void BuildTasks(const std::list<std::pair<Job,std::pair<mesos::Resources, mesos::SlaveID>>>& jobs, std::vector<mesos::TaskInfo>& tasks);
	void BuildWakeUpTasks(std::list<std::pair<mesos::Resources, mesos::SlaveID>>& locations, 
			std::vector<mesos::TaskInfo>& tasks);
	bool ISExecutorValid(const std::string& slaveID, const std::string& executorID);

private:
	core::ConcurrentDictionary<std::pair<SlaveID, ExecutorID>, Executor> m_executors;
	mesos::ExecutorInfo m_executorInfo;
	std::string m_role;
	core::ConcurrentDictionary<TaskID, Task> m_activatedTasks; 
	core::SyncQueue<Job> m_jobsQueue;
	core::TimedAsyncExecutor m_timedAsyncExec;
	std::unique_ptr<mesos::SchedulerDriver> m_mesosDriver;
	mutable std::mutex m_mutex;
};
