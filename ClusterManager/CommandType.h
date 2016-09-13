#pragma once

#include <string>
#include "Core/Enumeration.h"

class CommandType
{
public:
    enum Enumeration
    {
        LoadParams,
        Process,
        UnLoadParams,
        Terminate
    };
ENUMERATION(CommandType)
};
