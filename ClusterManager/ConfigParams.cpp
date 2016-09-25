#include "ConfigParams.h"
#include "Params.h"

using namespace std;

ConfigParams& ConfigParams::Instance()
{
	static ConfigParams instance;
	return instance;
}

void ConfigParams::Load(const Params& configParams)
{
	list<NodeAddress> processingUnitsAddresses = configParams.GetValue("Processing units address");
	int logicalProcessingUnitID = 0;
	//Addresses may return (more than one processing process per node)
	for(const NodeAddress& address : processingUnitsAddresses)
	{
		m_processingUnitsMap[address].push_back(logicalProcessingUnitID);
	}
	m_recoveryTime = configParams.GetValue("Recovery Time");
	m_hostAddress = StringConverter::Convert(configParams.GetValue("Host Address"));
	m_mesosMasterAddress = StringConverter::Convert(configParams.GetValue("Mesos Host Address"));
	m_mesosMasterPort = configParams.GetValue("Mesos Port Address");
	m_executableDir = StringConverter::Convert(configParams.GetValue("Execution Directory"));
}
