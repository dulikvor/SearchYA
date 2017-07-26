#pragma once

#include <unordered_map>
#include <memory>
#include "JobCreator.h"
#include "JobType.h"

class JobFactoryContainer {
public:
    static JobFactoryContainer& Instance();
    ~JobFactoryContainer();
    std::unique_ptr<Job> Create(JobType jobType, int id);


private:
    JobFactoryContainer();

private:
    std::unordered_map<JobType::Enumeration , IJobCreator*, JobType::Hash> m_factoryContainer;
};


