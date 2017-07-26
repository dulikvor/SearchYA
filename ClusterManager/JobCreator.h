#pragma once

#include <memory>
#include "Core/Exception.h"
#include "Job.h"

class IJobCreator
{
public:
    IJobCreator() = default;
    virtual ~IJobCreator() = default;
    virtual std::unique_ptr<Job> Create(int id){
        throw core::Exception(SOURCE, "None supported operation");
    }
};

template<typename T>
class JobCreator : public IJobCreator
{
public:
    JobCreator() = default;
    virtual ~JobCreator() = default;
   std::unique_ptr<Job> Create(int id) override {
        return std::unique_ptr<Job>(new T(id));
    }
};

