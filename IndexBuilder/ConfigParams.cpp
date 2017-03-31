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
	m_redisHostAddress = StringConverter::Convert(configParams.GetValue("Redis Server Address"));
}
