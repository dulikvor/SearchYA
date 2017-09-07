#include "JobFactoryContainer.h"
#include <utility>
#include "Core/Exception.h"
#include "InitJob.h"
#include "IndexingJob.h"
#include "GetTopKJob.h"

using namespace std;
using namespace core;

JobFactoryContainer& JobFactoryContainer::Instance() {
    static JobFactoryContainer instance;
    return instance;
}

JobFactoryContainer::~JobFactoryContainer(){
    for(auto& creator : m_factoryContainer){
        m_factoryContainer.erase(creator.first);
        delete creator.second;
    }
}

JobFactoryContainer::JobFactoryContainer() {
    m_factoryContainer.insert(make_pair(JobType::Init, new JobCreator<InitJob>()));
    m_factoryContainer.insert(make_pair(JobType::DocumentIndexing, new JobCreator<IndexingJob>()));
    m_factoryContainer.insert(make_pair(JobType::GetTopK, new JobCreator<GetTopKJob>()));
}

unique_ptr<Job> JobFactoryContainer::Create(JobType jobType, int id, GeneralParams const * const params) {
    auto it = m_factoryContainer.find(jobType);
    if(it != m_factoryContainer.end())
        return it->second->Create(id, params);
    else
        throw Exception(SOURCE, "None supported job type - %s was requested", jobType.ToString().c_str());
}