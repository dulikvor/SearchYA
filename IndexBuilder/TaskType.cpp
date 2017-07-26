#include "TaskType.h"
#include "Core/Exception.h"

using namespace core;

ENUMERATION_NAMING_BEGIN(TaskType)
	{TaskType::Enumeration::Init, "Init"},
	{TaskType::Enumeration::Indexing, "Indexing"},
	{TaskType::Enumeration::GetTopK, "Get Top K"}
ENUMERATION_NAMING_END(TaskType)
