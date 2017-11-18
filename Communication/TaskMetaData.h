#pragma once

#include <utility>
#include <memory>
#include "Serializor.h"

struct TaskMetaData
{
public:
    typedef std::unique_ptr<char, std::default_delete<char[]>> Unique_Ptr;

public:
    TaskMetaData();
    TaskMetaData(std::pair<Unique_Ptr, int> tracingData,
        std::pair<Unique_Ptr, int> taskData);
    TaskMetaData(TaskMetaData&& obj);
    ~TaskMetaData(){}

    void Serialize(Serializor& context);
    static TaskMetaData Deserialize(Serializor& context);
public:
    std::pair<Unique_Ptr, int> TracingData;
    std::pair<Unique_Ptr, int> TaskData;
};

