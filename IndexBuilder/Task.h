#pragma once

#include <string>
#include <functional>
#include <vector>
#include "TaskState.h"

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
	Task(const std::string& id,  float coreCount):
		m_id(id), m_coreCount(coreCount){}
	virtual ~Task(){}
	//Task entry point, from here the task will initiate its run.
	virtual void Run(){
		throw core::Exception(SOURCE, "Operation is not supported.");
	}
	//Properties
	std::string GetID() const {return m_id;}

private:
	std::string m_id;
	float m_coreCount;
};
