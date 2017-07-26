#pragma once

#include "Job.h"

class InitJob : public Job
{
public:
    InitJob(int id): Job(id, 0.1){}
    virtual ~InitJob(){}
    std::unique_ptr<std::pair<const char*, int>, Deleter> GenerateTaskData() const override;
    const char* GetLabel() const override{
        return "Init"; }
};

