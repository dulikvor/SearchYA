#pragma once

#include "Job.h"

class IndexingJob : public Job
{
public:
    IndexingJob(int id): Job(id, 0.1){}
    virtual ~IndexingJob(){}
    const char* GetLabel() const override{
        return "Indexing";
    }
};


