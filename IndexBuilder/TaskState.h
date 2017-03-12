#pragma once

#include <string>
#include "Core/Enumeration.h"

class TaskState
{
public:
	TaskState();
    enum Enumeration
    {
		Starting = 0,
        Running,
		Finished,
		Failed
    };
ENUMERATION(TaskState)
};
