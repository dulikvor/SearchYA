#pragma once

#include <list>
#include <memory>
#include <atomic>
#include <string>
#include <mutex>
#include <ctime>
#include <execinfo.h>
#include "Source.h"

namespace spdlog
{
	class logger;
}
namespace core
{
	class TraceListener;
}

enum TraceSeverity
{
	Verbose = 1,
	Info,
	Fatal = 4
};

const int ORIGINAL_STACK_FRAMES_GUESS = 20; //20 is arbitrary

namespace core
{
	class Logger
	{
	public:
		static Logger& Instance();
		~Logger(); //add nullptr to release the blocked wait. not by cancel of the thread.
		//Receives a new listener, a reference to the listener will be kept under the logger.
		void AddListener(const std::shared_ptr<TraceListener>& listener);
		//Builds a formatted message from a received ansi like format, matching arguments and 
		//the print source location. all will be used to genreate a string instance containing
		//the final result.
		std::string FormatMessage(const Source& source, const char* format, ...);
		//
		void Start(TraceSeverity severity);
		//
		template<typename... Args>
		void Trace(TraceSeverity severity, const Source& source, const char* format, Args... args)
		{
			if(severity < m_severity)
				return;
			std::string message = FormatMessage(source, format, args...);
			Log(severity, message);
		}


		template<typename ... Args>
		void PrintStack(const Source& source, const char* format, Args... args)
		{
			//Trace the first message:
			Trace(TraceSeverity::Fatal, source, format, args...);
			//Trace the entire stack frames:
			//Get the stack frames data
			void* stackFramesAddresses = malloc(sizeof(void*)*ORIGINAL_STACK_FRAMES_GUESS);
			int stackFramesSize = ORIGINAL_STACK_FRAMES_GUESS;
			int readFramesCount;
			while(stackFramesSize == (readFramesCount = backtrace(&stackFramesAddresses, stackFramesSize)))
			{
				free(stackFramesAddresses);
				stackFramesSize*=2;
				stackFramesAddresses = malloc(sizeof(void*)*stackFramesSize);
			}
			//Get the symbols
			char** stackFramesSymbols = backtrace_symbols(&stackFramesAddresses, readFramesCount);
			for(int index = 0; index < readFramesCount; index++)
			{
				Trace(TraceSeverity::Fatal, source, "%s", std::string(stackFramesSymbols[index]).c_str());
			}
			free(stackFramesSymbols);
		}
		//
		void Log(TraceSeverity severity, const std::string& message);
		//Flush the log.
		void Flush();
		//Properties
		TraceSeverity GetSeverity() const { return m_severity; }

	private:
		Logger();

	private:
		std::list<std::shared_ptr<TraceListener>> m_listeners;
		std::shared_ptr<spdlog::logger> m_logger;
		std::atomic_bool m_running;
		TraceSeverity m_severity;
		mutable std::mutex m_mut;
		const int Local_buffer_size = 2000;
	};
}

#define TRACE_IMPL(severity, format, ...)\
	if(severity >= core::Logger::Instance().GetSeverity()) \
		core::Logger::Instance().Trace(severity, SOURCE, format, ##__VA_ARGS__)

#define TRACE_INFO(format, ...) \
	TRACE_IMPL(TraceSeverity::Info, format, ##__VA_ARGS__)
#define TRACE_VERBOSE(format, ...) \
	TRACE_IMPL(TraceSeverity::Verbose, format, ##__VA_ARGS__)
