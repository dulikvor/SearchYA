#pragma once

#include <vector>
#include <memory>
#include "SyncQueue.h"
#include "Thread.h"

namespace core
{
	class AsyncTask;
}

namespace core
{
	class AsyncExecutor
	{
	public:
		explicit AsyncExecutor(int threadPoolSize = 1);
		~AsyncExecutor();

		virtual void SpawnTask(AsyncTask* task){m_taskQueue.Push(task);}

	private:
		void EntryPoint(void);

	private:
		std::vector<std::unique_ptr<Thread>> m_threadPool;
		SyncQueue<AsyncTask*> m_taskQueue;
	};
}
