#pragma once

#include "Core/Enumeration.h"

class MessageType
{
public:
	static const int HeaderSize = sizeof(int) * 2; //Type+Enum written as int
	MessageType();
    enum Enumeration
    {
		Init,
		InitAck
    };
ENUMERATION(MessageType)
};
