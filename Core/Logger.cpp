#include "Logger.h"
#include <functional>
#include <iostream>
#include "Thread.h"
#include "Exception.h"

using namespace std;

Logger& Logger::Instance()
{
    static Logger logger;
    return logger;
}

Logger::~Logger()
{
    m_queue.Push(nullptr);
    m_thread->Join(0);
}

Logger::Logger()
{
    m_thread.reset(new Thread("LoggerThread", bind(&Logger::HandleLog, this)));
}

void Logger::Push(const LogElement& logElement)
{
    m_queue.Push(new LogElement(logElement));
}

void Logger::Start(const std::string& logPath)
{
    m_logPath = logPath;
    m_thread->Start();
}

void Logger::HandleLog()
{
    try
    {
        while(1)
        {
            LogElement* logElement = m_queue.Pop();
            if(logElement ==  nullptr)
                break;
            cout<<logElement->message<<endl;
            delete logElement;
        }
    }
    catch(const Exception& e)
    {

    }
}
