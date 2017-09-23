#pragma once

#include "Task.h"

class GetTopKTask : public Task
{
public:
    GetTopKTask(void* const tag): Task(tag){}
    void ProcessTask(const std::string& data) override;
};

