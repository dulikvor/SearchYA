#include "Scheduler.h"
#include "ConfigParams.h"
#include "Pu.h"

using namespace std;

void Scheduler::Initialize(const Params& configParams)
{
	for(const ConfigParams::NodePUMap::value_type& pair : ConfigParams::Instance().GetProcessingUnitsMap())
	{
		m_processingUnits[pair.first].PushBack(Pu(pair.second));
	}
	for(const std::unordered_map<NodeAddress, OrderedList<Pu>>::value_type& pair : m_processingUnits)
	{
		m_processingUnits[pair.first].Sort();
	}
}
