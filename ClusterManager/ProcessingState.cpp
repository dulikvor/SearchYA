#include "ProcessingState.h"
#include <atomic>
#include "Core/Exception.h"
#include "Core/Logger.h"
#include "ClusterManager.h"
#include "JobFactoryContainer.h"

using namespace std;
using namespace core;

void ProcessingState::HandleState(StateContext& stateContext, CommandType commandType, const GeneralParams& params)
{
    switch(commandType)
    {
		case CommandType::Index:
        {
            HandleIndex(params);
            break;
        }
		case CommandType::GetTopK:
		{
			HandleGetTopK(params);
			break;
		}
		case CommandType::Terminate:
		{
			State::HandleTerminate();
			break;
		}
		case CommandType::Init:
        defualt:
        {
            throw core::Exception(SOURCE, "Unauthorized command was received - %s", commandType.ToString().c_str());
        }
    }
}

void ProcessingState::HandleIndex(const GeneralParams& params)
{
	static atomic_int id(0);
	unique_ptr<Job> job = JobFactoryContainer::Instance().Create(JobType::DocumentIndexing, id++);
	TRACE_INFO("New Indexing job %d was received", (int)id);
	ClusterManager::Instace().GetScheduler().AddJob(move(job));
}

void ProcessingState::HandleGetTopK(const GeneralParams &params) {
	static atomic_int id(0);
	unique_ptr<Job> job = JobFactoryContainer::Instance().Create(JobType::GetTopK, id++, &params);
	TRACE_INFO("New GetTopK job %d was received", (int)id);
	ClusterManager::Instace().GetScheduler().AddJob(move(job));
}

