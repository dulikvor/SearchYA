#include "Promise.h"
#include "AsyncTask.h"

using namespace std;

namespace core{
    void Promise::NotifyOnPromiseSet() {
        auto task = m_task.lock();
        task->NotifyOnPromiseSet();
    }
    std::shared_ptr<AsyncTask> Promise::GetTask(const std::function<void(void)>& requestedPoint) {
        auto task = make_shared<AsyncTask>(requestedPoint);
        m_task = task;
        return task;
    }
}
