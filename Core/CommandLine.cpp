#include "CommandLine.h"
#include <algorithm>
#include "Assert.h"
#include "Exception.h"

using namespace std;
using namespace core;

CommandLine::CommandLine(int argc, char** argv)
{
	Parse(argc, argv);
}

string& CommandLine::GetArgument(const string& argumentName)
{
	ArgumentList::iterator it = find_if(m_arguments.begin(), m_arguments.end(), [&argumentName]
			(const pair<string, string>& elem)->bool{return elem.first == argumentName;});
	if(it == m_arguments.end())
		throw Exception(SOURCE, "Invalid argument name was provided - %s", argumentName.c_str());
	return (*it).second;
}

void CommandLine::Parse(int argc, char** argv)
{
	for(int index = 0; index < argc; index++)
	{
		string argName, argValue;
		if(!ParseArgument(argv[index], argName, argValue))
			continue;
		m_arguments.push_back(make_pair(argName, argValue));
	}
}

bool CommandLine::ParseArgument(const char* argument, string& argName, string& argValue)
{
	if(argument[0] != '-' || argument[1] != '-')
		return false;
	int index = 2;
	int count = 0;
	while(argument[count + index] != '=' && argument[count + index] != '\0')
	{
		count++;
	}
	argName = string(argument + index, count);
	index += count + 1;
	count = 0;
	while(argument[count + index] != '\0')
	{
		count++;
	}
	argValue = string(argument + index, count);
	return true;
}
