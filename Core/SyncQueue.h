#pragma once

#include <queue>
#include <condition_variable>
#include <mutex>

template <typename T>
class SyncQueue
{
public:
    SyncQueue(){}
    ~SyncQueue(){}

    void Push(const T& element)
    {
        std::unique_lock<std::mutex>(m_mutex);
        m_queue.push(element);
        m_conditionVar.notify_one();
    }

    T Pop()
    {
        std::unique_lock<std::mutex> localLock(m_mutex);
        m_conditionVar.wait(localLock, [this]{return m_queue.empty() == false;});
        T ret = m_queue.front();
        m_queue.pop();
        return ret;
    }

private:
    std::queue<T> m_queue;
    mutable std::condition_variable m_conditionVar;
    mutable std::mutex m_mutex;
};
