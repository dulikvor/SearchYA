#pragma once

#include "Core/Enumeration.h"

class MessageType
{
public:
	static const int HeaderSize = sizeof(int) * 2;
	MessageType();
    enum Enumeration
    {
		Init,
		Discovery
    };
ENUMERATION(MessageType)
};
