#pragma once

#include <memory>
#include "SyncQueue.h"
#include "LogElement.h"

namespace Core
{
	class Thread;
}

class Logger
{
public:
    static Logger& Instance();
    ~Logger(); //add nullptr to release the blocked wait. not by cancel of the thread.

    void Push(const LogElement& logElement);
    void Start(const std::string& logPath);

private:
    Logger();
    void HandleLog();
private:
    std::unique_ptr<Core::Thread> m_thread;
	Core::SyncQueue<LogElement*> m_queue;
    std::string m_logPath;
};
