#pragma

#include <list>
#include <string>
#include "Task.h"
#include "TaskState.h"
#include "StateContext.h"

class ProcessingManager;
namespace core
{
	class AsyncTask;
}
//Represents a computation task. the task includes various states and a given handler
//per state. will hold all created async tasks (which provides the resource to carry the
//state transition and the buisness logic of execution). will take responsibility for the
//life duration of the given async tasks.
class IndexingTask : public Task
{
public:
	IndexingTask(ProcessingManager& manager, const std::string& id,  int coreCount):
		Task(manager, id, coreCount){}
	virtual ~IndexingTask(){}
	//Task entry point, from here the task will initiate its run.
	void Run(std::function<void(void)>* const specificOnInit) override;

private:
	//Task init state hander, will inform the executor with the transition to running state.
	//will submit created processing tasks to the processing manager.
	void OnInit(std::vector<std::function<void(void)>>* const specificOnProcessingList) override;
	//Task processing state handler. will handle all needed processing and transit to the next
	//stage upon completion. calls are bound to the processing async executor, state transition
	//to the regular single threaded async executor.
	void OnProcessing(int subTaskID, std::function<void(void)>* const processingFunctio) override;
	void IndexDocument();
};
