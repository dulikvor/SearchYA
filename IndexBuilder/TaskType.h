#pragma once

#include "Core/src/Enumeration.h"

class TaskType
{
public:
    enum Enumeration
    {
		Init,
        Indexing,
        GetTopK
    };
ENUMERATION(TaskType)
};
