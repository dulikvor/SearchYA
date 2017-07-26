#pragma once

#include <unordered_map>
#include <memory>
#include "TaskCreator.h"
#include "TaskType.h"

class ProcessingManager;

class TasksFactoryContainer {
public:
    ~TasksFactoryContainer();
    static TasksFactoryContainer& Instance();
    std::unique_ptr<Task> Create(TaskType taskType,
        const std::string& id,  float coreCount, const char* data, int length);

private:
    TasksFactoryContainer();

private:
    std::unordered_map<TaskType, ITaskCreator*, TaskType::Hash> m_factoryContainer;
};


