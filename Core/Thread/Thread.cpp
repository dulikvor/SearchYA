#include "Thread.h"
#include "Exception/Assert.h"
#include <iostream>
using namespace std;

Thread::Thread(const std::string& threadName, const std::function<void(void)>& requestedPoint)
    : m_name(threadName), m_requestedPoint(requestedPoint)
{
    ASSERT((int)m_name.size() <= MAX_THREAD_NAME, "Thread name is in invalid size");
}

void Thread::Start()
{
    LINUX_VERIFY(pthread_create(&m_id, nullptr, EntryPoint, (void*)&m_requestedPoint) == 0);
    LINUX_VERIFY(pthread_setname_np(m_id, m_name.c_str()) == 0);
}

void Thread::Join(int miliSecondsWait)
{
    LINUX_VERIFY(pthread_join(m_id, nullptr) == 0);
}

void* Thread::EntryPoint(void* requestedEntryPoint)
{
    try
    {
        function<void(void)>* entryPoint = (function<void(void)>*)requestedEntryPoint;
        (*entryPoint)();
    }
    catch(const Exception& e)
    {
    }
    return nullptr;
}
