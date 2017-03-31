#include "CommandType.h"
#include "Core/Exception.h"

using namespace core;

ENUMERATION_NAMING_BEGIN(CommandType)
	{CommandType::Enumeration::Init, "Init"},
	{CommandType::Enumeration::Process, "Process"},	
	{CommandType::Enumeration::Terminate, "Terminate"},
	{CommandType::Enumeration::Failed, "Failed"}
ENUMERATION_NAMING_END(CommandType)
