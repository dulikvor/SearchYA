#pragma once

#include <vector>
#include <memory>
#include "SyncQueue.h"
#include "Thread.h"

class AsyncTask;

class AsyncExecutor
{
public:
    explicit AsyncExecutor(int thredPoolSize = 1);
    ~AsyncExecutor();

    void SpawnTask(AsyncTask* task){m_taskQueue.Push(task);}

private:
    void EntryPoint(void);

private:
    std::vector<Thread> m_threadPool;
    SyncQueue<AsyncTask*> m_taskQueue;
};
