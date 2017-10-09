#pragma once

#include <memory>
#include <string>
#include "Core/src/Exception.h"

class AsyncService;

class Task
{
public:
	Task(void* const tag);
	virtual	void ProcessTask(const std::string& data){
		throw core::Exception(SOURCE, "Function is not supported");
	}
	virtual ~Task(){}
protected:
	std::shared_ptr<AsyncService> m_service;
};
