#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <mesos/scheduler.hpp>
#include "Core/OrderedList.h"
#include "ConfigParams.h"
#include "Pu.h"

class Scheduler// : public mesos::Scheduler
{
public:
	Scheduler(){}
	~Scheduler(){}
	//Initialize will address the config data reading the cluster information, the different processing units locations
	//indicated by thier node address (they addresses may return). the function will create an internal object representing
	//each processing unit, storing each process by its location address, no need to sort the most busy processes, since they are all idle
	//at the moment.
	void Initialize(const Params& configParams);

private:
	void InitializeMesos();

private:
	std::unordered_map<ConfigParams::NodeAddress, OrderedList<Pu>> m_processingUnits;
	std::unique_ptr<mesos::SchedulerDriver> m_mesosDriver;
	mesos::FrameworkInfo m_frameWorkInfo;
	mesos::ExecutorInfo m_executorInfo;
};
