#include "TasksFactoryContainer.h"
#include <utility>
#include "Core/src/Exception.h"
#include "InitTask.h"
#include "IndexingTask.h"
#include "TopKTask.h"

using namespace std;
using namespace core;

TasksFactoryContainer::~TasksFactoryContainer() {
    for(auto& creator : m_factoryContainer) {
        m_factoryContainer.erase(creator.first);
        delete creator.second;
    }
}

TasksFactoryContainer& TasksFactoryContainer::Instance() {
    static TasksFactoryContainer instance;
    return instance;
}

TasksFactoryContainer::TasksFactoryContainer() {
    m_factoryContainer.insert(make_pair(TaskType::Init, new TaskCreator<InitTask>()));
    m_factoryContainer.insert(make_pair(TaskType::Indexing, new TaskCreator<IndexingTask>()));
    m_factoryContainer.insert(make_pair(TaskType::GetTopK, new TaskCreator<TopKTask>()));
}

unique_ptr<Task> TasksFactoryContainer::Create(TaskType taskType,
        const std::string& id,  float coreCount, const char* data, int length) {
    auto it = m_factoryContainer.find(taskType);
    if(it != m_factoryContainer.end())
        return it->second->Create(id, coreCount, data, length);
    else
        throw Exception(SOURCE, "None supported job type - %s was requested", taskType.ToString().c_str());
}
