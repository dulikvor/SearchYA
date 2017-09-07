#pragma once

#include <memory>
#include "Core/Exception.h"
#include "Job.h"

class GeneralParams;

class IJobCreator
{
public:
    IJobCreator() = default;
    virtual ~IJobCreator() = default;
    virtual std::unique_ptr<Job> Create(int id, GeneralParams const * const params){
        throw core::Exception(SOURCE, "None supported operation");
    }
};

template<typename T>
class JobCreator : public IJobCreator
{
public:
    JobCreator() = default;
    virtual ~JobCreator() = default;
   std::unique_ptr<Job> Create(int id, GeneralParams const * const params) override {
        return std::unique_ptr<Job>(new T(id, params));
    }
};

