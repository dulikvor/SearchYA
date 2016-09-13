#pragma once

#include "CommandType.h"
//Params will be added in the future
class Params;

struct Command
{
    Command(CommandType newCommandType, const Params& newParams): commandType(newCommandType), params(newParams){}

    CommandType commandType;
    const Params& params;
};
