#include "Promise.h"
#include "AsyncTask.h"

using namespace std;

namespace core{
    void Promise::NotifyOnPromiseSet() {
        auto task = m_task.lock();
        task->NotifyOnPromiseSet();
    }
    std::shared_ptr<AsyncTask> Promise::GetTask(const std::function<void(void)>& requestedPoint) {
        shared_ptr<Promise> promise = shared_ptr<Promise>(this);
        auto task = make_shared<AsyncTask>(requestedPoint, promise);
        m_task = task;
        return task;
    }
}
