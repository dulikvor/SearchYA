#pragma once

#include <string>
#include "Enumeration/Enumeration.h"

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
