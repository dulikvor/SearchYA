#pragma once

#include <memory>
#include <string>
#include "RedisSearchModule/Document.h"
#include "Core/src/Promise.h"
#include "Task.h"
#include "TaskState.h"

class TopKTask : public Task
{
public:
    TopKTask(const std::string& id,  float coreCount, const char* data, int length);
    virtual ~TopKTask(){}

    void Run() override;

private:
    std::string m_word;
    int m_k;
    std::vector<Document> m_documents;
};

