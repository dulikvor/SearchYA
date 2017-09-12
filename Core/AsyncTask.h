#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <string>

namespace core
{
	class Promise;

	class AsyncTask
	{
	public:
		explicit AsyncTask(const std::function<void(void)>& requestedPoint);
		AsyncTask():m_state(AsyncTaskState::CREATED){}
		AsyncTask(const AsyncTask& obj);
		AsyncTask& operator=(const AsyncTask& obj);
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
		void ChangeState(AsyncTaskState newState){
			m_state = newState;
		}
		void SetFailureReason(const std::string failureReason){m_failureReason = failureReason;}
		void SetRequestedEntryPoint(const std::function<void(void)>& requestedPoint){
			m_requestedEntryPoint = requestedPoint;
		}
		AsyncTaskState GetState() const {
			return m_state;
		}
		std::string GetFailureReason() const {
			return m_failureReason;
		}
		std::function<void(void)> GetRequestedEntryPoint() const{
			return m_requestedEntryPoint;
		}
		std::shared_ptr<Promise> GetPromise() const{
			return m_promise;
		}
		void SetPromise(const std::shared_ptr<Promise>& promise){
			m_promise = promise;
		}
		void Start();
		void Wait();
		const Promise& Get();
		void NotifyOnFailure();

	private:
		friend class Promise;
		void NotifyOnPromiseSet();

	private:
		std::function<void(void)> m_requestedEntryPoint;
		std::shared_ptr<Promise> m_promise;
		AsyncTaskState m_state;
		mutable std::condition_variable m_waitCv;
		mutable std::mutex m_waitMut;
		mutable std::condition_variable m_promiseCv;
		mutable std::mutex m_promiseMut;
		std::string m_failureReason;
	};
}
