#include "TaskState.h"

ENUMERATION_NAMING_BEGIN(TaskState)
	{TaskState::Enumeration::Starting, "Starting"},
	{TaskState::Enumeration::Running, "Running"},	
	{TaskState::Enumeration::Finished, "Finished"},
	{TaskState::Enumeration::Failed, "Failed"}
ENUMERATION_NAMING_END(TaskState)
