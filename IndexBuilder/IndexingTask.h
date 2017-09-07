#pragma once

#include <string>
#include "Task.h"
#include "TaskState.h"

//Represents a computation task. the task includes various states and a given handler
//per state. will hold all created async tasks (which provides the resource to carry the
//state transition and the buisness logic of execution). will take responsibility for the
//life duration of the given async tasks.
class IndexingTask : public Task
{
public:
	IndexingTask(const std::string& id,  float coreCount, const char* data, int length):
		Task(id, coreCount){}
	virtual ~IndexingTask(){}
	//Task entry point, from here the task will initiate its run.
	void Run() override;
};
