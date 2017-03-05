#include "TaskState.h"
#include <mesos/mesos.hpp>

TaskState::TaskState()
{
	static_assert((int)mesos::TaskState::TASK_STARTING == (int)Starting, "Invalid task state enum value");
	static_assert((int)mesos::TaskState::TASK_RUNNING == (int)Running, "Invalid task state enum value");
	static_assert((int)mesos::TaskState::TASK_FINISHED == (int)Finished, "Invalid task state enum value");
	static_assert((int)mesos::TaskState::TASK_FAILED == (int)Failed, "Invalid task state enum value");
}

ENUMERATION_NAMING_BEGIN(TaskState)
	{TaskState::Enumeration::Starting, "Starting"},
	{TaskState::Enumeration::Running, "Running"},	
	{TaskState::Enumeration::Finished, "Finished"},
	{TaskState::Enumeration::Failed, "Failed"}
ENUMERATION_NAMING_END(TaskState)
