#include "ProcessingState.h"
#include <atomic>
#include "Core/Exception.h"
#include "Core/Logger.h"
#include "ClusterManager.h"
#include "Job.h"

using namespace std;
using namespace core;

void ProcessingState::HandleState(StateContext& stateContext, CommandType commandType, const GeneralParams& params)
{
    switch(commandType)
    {
		case CommandType::Process:
        {
            HandleProcess(params);
            break;
        }
		case CommandType::InitAck:
		{
			//Do nothing, may happen due to race condition with in mesos.
			break;
		}
		case CommandType::Terminate:
		{
			State::HandleTerminate();
			break;
		}
        defualt:
        {
            throw core::Exception(SOURCE, "Unauthorized command was received - %s", commandType.ToString().c_str());
        }
    }
}

void ProcessingState::HandleProcess(const GeneralParams& params)
{
	static atomic_int id(0);
	Job job = {1, id++};
	TRACE_INFO("New job %d was received", (int)id);
	ClusterManager::Instace().GetScheduler().AddJob(job);
}

