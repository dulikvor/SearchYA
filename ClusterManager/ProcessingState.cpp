#include "ProcessingState.h"
#include "Core/Exception.h"
#include "ClusterManager.h"
#include "Job.h"

using namespace std;

void ProcessingState::HandleState(StateContext& stateContext, CommandType commandType, const Params& params)
{
    switch(commandType)
    {
		case CommandType::Process:
        {
            HandleProcess(params);
            break;
        }
        defualt:
        {
            throw Core::Exception(SOURCE, "Unauthorized command was received - %s", commandType.ToString().c_str());
        }
    }
}

void ProcessingState::HandleProcess(const Params& params)
{
	Job job;
	ClusterManager::Instace().GetScheduler().AddJob(job);
}
