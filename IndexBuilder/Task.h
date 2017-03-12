#pragma

#include <list>
#include <string>
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
class Task
{
public:
	Task(ProcessingManager& manager, const std::string& id,  int coreCount):
		m_state(TaskState::Starting), m_manager(manager), m_id(id), m_coreCount(coreCount),
		m_failure(false){}
	~Task();
	//Task entry point, from here the task will initiate its run.
	void Run();
	//Properties
	TaskState GetState() const {return m_state;}
	int GetCoreCount() const {return m_coreCount;}
	std::string GetID() const {return m_id;}
	bool Failure() const {return m_failure;}
	std::string GetREason() const {return m_reason;}


private:
	//Task init state hander, will inform the executor with the transition to running state.
	//will submit created processing tasks to the processing manager.
	void OnInit();
	//Task processing state handler. will handle all needed processing and transit to the next
	//stage upon completion. calls are bound to the processing async executor, state transition
	//to the regular single threaded async executor.
	void OnProcessing(int id);
	//Will be called upon every completed processing task, if the given processing task is the
	//last one, an update to mesos will be conducted and a transition to deletion will be made.
	void OnFinished(int id);


private:
	std::list<core::AsyncTask*> m_taskActivities;
	std::string m_id;
	ProcessingManager& m_manager;
	TaskState m_state;
	StateContext m_stateMachine;
	int m_coreCount;
	bool m_failure;
	std::string m_reason;
	int m_processingTaskCount;
};
