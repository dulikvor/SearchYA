#pragma once

#include "Job.h"

class GeneralParams;

class InitJob : public Job
{
public:
    InitJob(int id, GeneralParams const * const params): Job(id, 0.1){}
    virtual ~InitJob(){}
    std::unique_ptr<std::pair<const char*, int>, Deleter> GenerateTaskData() const override;
    const char* GetLabel() const override{
        return "Init"; }
};

