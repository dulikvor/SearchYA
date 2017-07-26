#pragma once

#include "Core/Enumeration.h"

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
