#include "TaskType.h"
#include "Core/src/Exception.h"

using namespace core;

ENUMERATION_NAMING_BEGIN(TaskType)
	{TaskType::Enumeration::Init, "Init"},
	{TaskType::Enumeration::Indexing, "Indexing"},
	{TaskType::Enumeration::GetTopK, "Get Top K Documents"}
ENUMERATION_NAMING_END(TaskType)
