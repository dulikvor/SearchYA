#pragma once

#include <string>
#include "Core/Enumeration.h"

class TaskState
{
public:
    enum Enumeration
    {
		Starting,
        Running,
		Finished,
		Failed
    };
ENUMERATION(TaskState)
};
