#include "AsyncTask.h"
#include "Exception.h"
#include "LogDefs.h"

using namespace std;

namespace Core
{
	void AsyncTask::Start()
	{
		{
			unique_lock<std::mutex> localLock(m_mutex);
			m_state = AsyncTaskState::RUNNING;
		}
		m_requestedEntryPoint();
		{
			unique_lock<std::mutex> localLock(m_mutex);
			m_state = AsyncTaskState::COMPLETED;
		}
		m_conditionVar.notify_one();
	}


	void AsyncTask::Wait()
	{
		unique_lock<std::mutex> localLock(m_mutex);
		m_conditionVar.wait(localLock, [&]{return m_state == AsyncTaskState::CANCELED ||
					m_state == AsyncTaskState::COMPLETED;});
		if(m_state == AsyncTaskState::CANCELED)
			throw Exception(SOURCE, m_failureReason);
	}

	void AsyncTask::NotifyOnFailure()
	{
		unique_lock<std::mutex> localLock(m_mutex);
		m_state = AsyncTaskState::CANCELED;
		m_conditionVar.notify_one();
	}
}

