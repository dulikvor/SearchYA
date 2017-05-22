#include "CommandType.h"
#include "Core/Exception.h"

using namespace std;
using namespace core;

ENUMERATION_NAMING_BEGIN(CommandType)
	{CommandType::Enumeration::Init, "Init"},
	{CommandType::Enumeration::InitAck, "InitAck"},
	{CommandType::Enumeration::Index, "Indexing"},	
	{CommandType::Enumeration::Terminate, "Terminate"}
ENUMERATION_NAMING_END(CommandType)

CommandType CommandType::FromMessageType(MessageType messageType)
{
	switch(messageType)
	{
		case MessageType::Init:
			{
				return CommandType::Init;
				break;
			}
		case MessageType::InitAck:
			{
				return CommandType::InitAck;
				break;
			}
		defualt:
			{
				throw Exception(SOURCE, "None supported type %s", messageType.ToString().c_str());
			}
	}
}
