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
	list<NodeAddress> processingUnitsAddresses = configParams("Processing units address");
	int logicalProcessingUnitID = 0;
	//Addresses may return (more than one processing process per node)
	for(const NodeAddress& address : processingUnitsAddresses)
	{
		m_processingUnitsMap[address].push_back(logicalProcessingUnitID);
	}
}
