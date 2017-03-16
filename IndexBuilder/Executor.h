#pragma once

#include <mutex>
#include <memory>
#include "mesos/executor.hpp"
#include "TaskState.h"

class GeneralParams;

class Executor : public mesos::Executor
{
public:
	Executor(){}
	virtual ~Executor();
	//
	void Init();

	//Mesos executor interface
  	void registered(mesos::ExecutorDriver* driver, const mesos::ExecutorInfo& executorInfo,
      const mesos::FrameworkInfo& frameworkInfo, const mesos::SlaveInfo& slaveInfo) override;
  	void reregistered(mesos::ExecutorDriver* driver, 
			const mesos::SlaveInfo& slaveInfo) override;
	//Will try to initiate a termination task in order to end the process (won't stand living
	//with out mesos :( ).
  	void disconnected(mesos::ExecutorDriver* driver) override;
	//
	void launchTask(mesos::ExecutorDriver* driver, const mesos::TaskInfo& task) override;
	//
	void killTask(mesos::ExecutorDriver* driver, const mesos::TaskID& taskId) override;
	//
  	void frameworkMessage(mesos::ExecutorDriver* driver, const std::string& data) override;
	//Will try to initiate a termination task in order to end the process.
	void shutdown(mesos::ExecutorDriver* driver) override;
	//
  	void error(mesos::ExecutorDriver* driver, const std::string& message) override;
	//Will inform mesos with the current state of a given task.
	void UpdateTaskStatus(const std::string& taskID, TaskState state);

	void OnTaskComplete(const std::string& taskID);

	void SendMessage(const std::string& data);

private:
	void SendWakeUpReply(const GeneralParams& params);

private:
	std::unique_ptr<mesos::MesosExecutorDriver> m_driver;
};
