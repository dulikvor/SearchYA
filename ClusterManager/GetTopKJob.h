#pragma once

#include <string>
#include <memory>
#include "Job.h"

class GeneralParams;

class GetTopKJob : public Job
{
public:
    GetTopKJob(int id, GeneralParams const * const params);
    virtual ~GetTopKJob(){}
    std::unique_ptr<std::pair<const char*, int>, Deleter> GenerateTaskData() const override;
    const char* GetLabel() const override{
        return "Get Top K Documents";
    }

private:
    std::string m_word;
    int m_k;
};

