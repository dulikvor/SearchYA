#pragma

#include <list>
#include <string>
#include "Task.h"
#include "TaskState.h"

class ProcessingManager;
namespace core
{
    class AsyncTask;
}
//Represents a computation task. the task includes various states and a given handler
//per state. will hold all created async tasks (which provides the resource to carry the
//state transition and the buisness logic of execution). will take responsibility for the
//life duration of the given async tasks.
class InitTask : public Task
{
public:
    InitTask(const std::string& id,  float coreCount, const char* data, int length);
    virtual ~InitTask(){}
    void Run() override;

private:
    std::string m_dbServerAddress;
};

