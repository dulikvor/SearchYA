#include "AsyncExecutor.h"
#include <string>
#include "Exception.h"
#include "AsyncTask.h"

using namespace std;

AsyncExecutor::AsyncExecutor(int threadPoolSize)
{
    for(int index = 0; index < threadPoolSize; index++)
    {
        m_threadPool.push_back(Thread(string("AsyncExec_") + to_string(index), std::bind(&AsyncExecutor::EntryPoint, this)));
    }
}

AsyncExecutor::~AsyncExecutor()
{
    for(int index = 0; index < (int)m_threadPool.size(); index++)
    {
        m_taskQueue.Push(nullptr);
    }
    for(Thread& thread : m_threadPool)
    {
        thread.Join();
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
    }
}
