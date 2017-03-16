#include "CommandType.h"
#include "Core/Exception.h"

using namespace std;
using namespace core;

ENUMERATION_NAMING_BEGIN(CommandType)
	{CommandType::Enumeration::Discovery, "Discovery"},
	{CommandType::Enumeration::Init, "Init"},
	{CommandType::Enumeration::Process, "Process"},	
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
		case MessageType::Discovery:
			{
				return CommandType::Discovery;
				break;
			}
		defualt:
			{
				throw Exception(SOURCE, "None supported type %s", messageType.ToString().c_str());
			}
	}
}
