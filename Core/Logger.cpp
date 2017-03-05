#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "spdlog/spdlog.h"
#include "spdlog/async_logger.h"
#include "Assert.h"
#include "Exception.h"
#include "TraceListener.h"

using namespace std;
using namespace core;
using namespace spdlog;
using namespace spdlog::sinks;
using namespace spdlog::level;

namespace core
{
	Logger& Logger::Instance()
	{
		static Logger logger;
		return logger;
	}

	Logger::~Logger()
	{
	}

	Logger::Logger()
	{
		static_assert((int)TraceSeverity::Verbose == (int)level_enum::debug, "TraceSeverity::Verbose dosn't match spdlog debug");
		static_assert((int)TraceSeverity::Info == (int)level_enum::info, "TraceSeverity::Info dosn't match spdlog info");
		static_assert((int)TraceSeverity::Fatal == (int)level_enum::err, "TraceSeverity::Error dosn't match spdlog err");

	}

	void Logger::Start(TraceSeverity severity)
	{
		ASSERT(!m_running.exchange(true));
		vector<shared_ptr<sink>> sinks;
		lock_guard<mutex> lock(m_mut);	
		{
			for(const shared_ptr<TraceListener>& listener : m_listeners)
			{
				sinks.emplace_back(listener->GetSink());
			}
		}
		m_logger = make_shared<async_logger>("Logger", sinks.begin(), sinks.end(), 4096, 
			async_overflow_policy::discard_log_msg);
		m_logger->set_level(level_enum(severity));
		m_severity = severity;
	}
	void Logger::Log(TraceSeverity severity, const string& message)
	{
		m_logger->log((level_enum)severity, message.c_str());
	}

	void Logger::Flush()
	{
		m_logger->flush();
	}

	void Logger::AddListener(const shared_ptr<TraceListener>& listener)
	{
		lock_guard<mutex> lock(m_mut);
		m_listeners.emplace_back(listener);
	}

	string Logger::FormatMessage(const Source& source, const char* format, ...)
	{
		va_list arguments;
		va_start(arguments, format);
		string result;

		char buf[Local_buffer_size] = "";
		int size = snprintf(buf, Local_buffer_size, "%s:%s:%d\t", source.file, 
			source.function, source.line);
		ASSERT(size < Local_buffer_size);
		size += vsnprintf(buf + size, Local_buffer_size - size, format, arguments);
		if(size < Local_buffer_size)
			result = buf;
		else //message was trunced or operation failed
		{
			int bufferSize = max(size, 32 * 1024);
			vector<char> largerBuf;
			largerBuf.resize(bufferSize);
			int largerSize = snprintf(&largerBuf[0], bufferSize, "%s:%s:%d\t", source.file, 
				source.function, source.line);
			ASSERT(largerSize < bufferSize);
			vsnprintf(&largerBuf[largerSize], bufferSize - largerSize, format, arguments);
			result = string(largerBuf.begin(), largerBuf.end());
		}

		va_end(arguments);
		return result;
	}
}
