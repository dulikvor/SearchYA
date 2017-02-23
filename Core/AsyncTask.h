#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <string>

namespace core
{
	class AsyncTask
	{
	public:
		explicit AsyncTask(const std::function<void(void)>& requestedPoint):
			m_requestedEntryPoint(requestedPoint), m_state(AsyncTaskState::CREATED){}
		~AsyncTask(){}

	public:
		enum class AsyncTaskState
		{
			CREATED,
			RUNNING,
			CANCELED,
			COMPLETED
		};

	public:
		//Properties
		void ChangeState(AsyncTaskState newState){std::unique_lock<std::mutex>(m_mutex); m_state = newState;}
		void SetFailureReason(const std::string failureReason){m_failureReason = failureReason;}

		void Start();
		void Wait();
		void NotifyOnFailure();

	private:
		std::function<void(void)> m_requestedEntryPoint;
		AsyncTaskState m_state;
		mutable std::condition_variable m_conditionVar;
		mutable std::mutex m_mutex;
		std::string m_failureReason;
	};
}
