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
	m_redisHostAddress = "127.0.0.1"; //StringConverter::Convert(configParams.GetValue("Redis Host Address"));
}
