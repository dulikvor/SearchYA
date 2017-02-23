#include "Thread.h"
#include <iostream>
#include "Assert.h"

using namespace std;

namespace core
{
	Thread::Thread(const string& threadName, const function<void(void)>& requestedPoint)
		: m_name(threadName), m_requestedPoint(requestedPoint)
	{
		m_running = false;
		ASSERT((int)m_name.size() <= MAX_THREAD_NAME);
	}

	void Thread::Start()
	{
		ASSERT(m_running.exchange(true) == false);
		m_thread.reset(new thread(bind(&Thread::EntryPoint, this)));
	}

	void Thread::Join() const
	{
		m_thread->join();
	}

	void Thread::EntryPoint()
	{
		try
		{
			m_requestedPoint();
		}
		catch(const Exception& e)
		{
		}
	}
}
