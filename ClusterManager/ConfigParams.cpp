#include "ConfigParams.h"
#include "Communication/GeneralParams.h"

using namespace std;

ConfigParams& ConfigParams::Instance()
{
	static ConfigParams instance;
	return instance;
}

void ConfigParams::Load(const GeneralParams& configParams)
{
	m_clusterNodesAddresses  = {"127.0.0.1"}; //configParams.GetValue("Processing units address");
	m_recoveryTime = 0; //configParams.GetValue("Recovery Time");
	m_hostAddress = "127.0.0.1"; //StringConverter::Convert(configParams.GetValue("Host Address"));
	m_mesosMasterAddress = "127.0.0.1"; //StringConverter::Convert(configParams.GetValue("Mesos Host Address"));
	m_executableDir = "/home/dudilikvornik/Projects/SearchYA/IndexBuilder/bin";//StringConverter::Convert(configParams.GetValue("Execution Directory"));
	m_role = "*"; //stringConverter::Convert(configParams.GetValue("Role"));
	m_redisServerAddress = "127.0.0.1";
}
