#pragma once

#include <string>
#include "Core/src/Enumeration.h"

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
