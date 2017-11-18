#include "TaskMetaData.h"

using namespace std;

TaskMetaData::TaskMetaData(): TracingData(make_pair(Unique_Ptr(nullptr), 0)), TaskData(make_pair(Unique_Ptr(nullptr), 0)){}
TaskMetaData::TaskMetaData(std::pair<Unique_Ptr, int> tracingData, std::pair<Unique_Ptr, int> taskMetaData)
    :TracingData(move(tracingData)), TaskData(move(taskMetaData)){}

TaskMetaData::TaskMetaData(TaskMetaData&& obj)
{
    TracingData = move(obj.TracingData);
    TaskData = move(obj.TaskData);
}

void TaskMetaData::Serialize(Serializor &context)
{
    Serializor::Serialize(context, pair<const char*, int>(TracingData.first.get(), TracingData.second));
    Serializor::Serialize(context, pair<const char*, int>(TaskData.first.get(), TaskData.second));
}

TaskMetaData TaskMetaData::Deserialize(Serializor &context)
{
    auto tracingData = Serializor::DeserializeCTypeString(context);
    auto taskData = Serializor::DeserializeCTypeString(context);
    return move(TaskMetaData(move(tracingData), move(taskData)));
}
