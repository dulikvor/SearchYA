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
	list<NodeAddress> processingUnitsAddresses = {"127.0.0.1"}; //configParams.GetValue("Processing units address");
	int logicalProcessingUnitID = 0;
	//Addresses may return (more than one processing process per node)
	for(const NodeAddress& address : processingUnitsAddresses)
	{
		m_processingUnitsMap[address].push_back(logicalProcessingUnitID);
	}
	m_recoveryTime = 0; //configParams.GetValue("Recovery Time");
	m_hostAddress = "127.0.0.1"; //StringConverter::Convert(configParams.GetValue("Host Address"));
	m_mesosMasterAddress = "127.0.0.1"; //StringConverter::Convert(configParams.GetValue("Mesos Host Address"));
	m_executableDir = "";//StringConverter::Convert(configParams.GetValue("Execution Directory"));
	m_role = "*"; //stringConverter::Convert(configParams.GetValue("Role"));
}
