#pragma once

#include "CommandType.h"
//Params will be added in the future
class GeneralParams;

struct Command
{
    Command(CommandType newCommandType, const GeneralParams& newParams): commandType(newCommandType), params(newParams){}

    CommandType commandType;
    const GeneralParams& params;
};
