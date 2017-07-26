#pragma once

#include "Job.h"

class GetTopKJob : public Job
{
public:
    GetTopKJob(int id): Job(id, 0.1){}
    virtual ~GetTopKJob(){}
    const char* GetLabel() const override{
        return "Get Top K Documents";
    }
};

