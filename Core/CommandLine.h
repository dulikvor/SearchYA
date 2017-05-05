#pragma once

#include <list>
#include <utility>
#include <string>

namespace core
{
	class CommandLine
	{
	public:
		CommandLine(int argc, char const** argv);
		~CommandLine(){}
		
		std::string& GetArgument(const std::string& argumentName);

	private:
		void Parse(int argc, char const** argv);
		bool ParseArgument(const char* argument, std::string& argName, std::string& argValue);

	private:
		using ArgumentList = std::list<std::pair<std::string, std::string>>;
		 ArgumentList m_arguments;

	};
}
