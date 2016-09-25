#include "Scheduler.h"

using namespace std;

void Scheduler::Initialize(const Params& configParams)
{
	for(const ConfigParams::NodePUMap::value_type& pair : ConfigParams::Instance().GetProcessingUnitsMap())
	{
		for(int id : pair.second)
			m_processingUnits[pair.first].PushBack(Pu(id));
	}
	for(const std::unordered_map<ConfigParams::NodeAddress, OrderedList<Pu>>::value_type& pair : m_processingUnits)
	{
		m_processingUnits[pair.first].Sort();
	}
	InitializeMesos();
}

void Scheduler::InitializeMesos()
{
	m_executorInfo.mutable_executor_id()->set_value("Executor");
	//m_executorInfo.set_type(mesos::ExecutorInfo::);
	m_executorInfo.mutable_command()->set_value(ConfigParams::Instance().GetExecDir() + "/Executor");
	m_executorInfo.set_name("Stub Executor");


	m_frameWorkInfo.set_user("");
	m_frameWorkInfo.set_name("Stub FrameWork");
	m_frameWorkInfo.set_failover_timeout(ConfigParams::Instance().GetRecoveryTime());
	m_frameWorkInfo.set_hostname(ConfigParams::Instance().GetHostAddress());
	string masterAddress = ConfigParams::Instance().GetMesosMasterAddress() + ":" + to_string(ConfigParams::Instance().GetMesosMasterPort());
	//m_mesosDriver.reset(new mesos::MesosSchedulerDriver(this, m_frameWorkInfo, masterAddress));
	m_mesosDriver->start();
}
