#pragma once

#include "TaskState.h"

class Task
{
public:
	explicit Task(): m_state(TaskState::Starting){}	
private:
	TaskState m_state;
};
