#include "AsyncExecutor.h"
#include <string>
#include <functional>
#include "Exception.h"
#include "AsyncTask.h"
#include "Assert.h"

using namespace std;

namespace core
{
	AsyncExecutor::AsyncExecutor(int threadPoolSize)
	{
		ASSERT(threadPoolSize == 1); //For now, temporary, support for more than one thread in thread pool is not supported.
		for(int index = 0; index < threadPoolSize; index++)
		{
			m_threadPool.emplace_back(new Thread(string("AsyncExec_") + to_string(index), bind(&AsyncExecutor::EntryPoint, this)));
			m_threadPool.back()->Start();
		}
	}

	AsyncExecutor::~AsyncExecutor()
	{
		for(int index = 0; index < (int)m_threadPool.size(); index++)
		{
			m_taskQueue.Push(nullptr);
		}
		for(const unique_ptr<Thread>& thread : m_threadPool)
		{
			thread->Join();
		}
	}

	void AsyncExecutor::EntryPoint()
	{
		while(1)
		{
			AsyncTask* task = m_taskQueue.Pop();
			if(task != nullptr)
			{
				try
				{
					task->Start();
				}
				catch(const Exception& e)
				{
					task->SetFailureReason(e.GetMessage());
					task->NotifyOnFailure();
				}
			}
			else
				break;
		}
	}
}

