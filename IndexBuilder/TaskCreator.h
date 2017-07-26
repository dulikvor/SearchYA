#pragma once

#include <memory>
#include "Core/Exception.h"
#include "Task.h"

class ITaskCreator
{
public:
    ITaskCreator() = default;
    virtual ~ITaskCreator() = default;
    virtual std::unique_ptr<Task> Create(const std::string& id,
         float coreCount, const char* data, int length){
            throw core::Exception(SOURCE, "None supported operation");
    }
};

template<typename T>
class TaskCreator : public ITaskCreator
{
public:
    TaskCreator() = default;
    virtual ~TaskCreator() = default;
    std::unique_ptr<Task> Create(const std::string& id,
         float coreCount, const char* data, int length){
        return std::unique_ptr<T>(new T(id, coreCount, data, length));
    }
};

