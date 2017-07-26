#pragma once

#include <memory>
#include <utility>
#include <string>
#include "Core/Exception.h"

class Job
{
public:
	struct Deleter{
		Deleter() = default;
		void operator()(std::pair<const char*, int>* obj){
			delete[] obj->first;
			delete obj;
		}
	};

public:
	Job() = default;
    Job(int id, float requiredCores): m_id(id), m_requiredCores(requiredCores){}
	virtual ~Job(){}
	virtual std::unique_ptr<std::pair<const char*, int>, Deleter> GenerateTaskData() const{
		return std::unique_ptr<std::pair<const char*, int>, Deleter>(
				new std::pair<const char*, int>(nullptr, 0), Deleter());
	};
	virtual const char* GetLabel() const{
		throw core::Exception(SOURCE, "Operation isn't supported");
	}

	float GetRequiredCores() const {return m_requiredCores;}
	int GetID() const {return m_id;}

private:
	float m_requiredCores;
	int m_id;
};
