#pragma once

#include <string>
#include "Core/Enumeration.h"
#include "Communication/MessageType.h"

class CommandType
{
public:
	static CommandType FromMessageType(MessageType messageType);
    enum Enumeration
    {
		Init,
        Process,
        Terminate,
		Failed
    };
ENUMERATION(CommandType)
};
