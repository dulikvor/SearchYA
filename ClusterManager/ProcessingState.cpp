#include "ProcessingState.h"
#include <atomic>
#include "Core/Exception.h"
#include "Core/Logger.h"
#include "Communication/GeneralParams.h"
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
		case CommandType::Discovery:
		{
			HandleDiscovery(params);
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
	Job job = {0.1, id++};
	TRACE_INFO("New job %d was received", (int)id);
	ClusterManager::Instace().GetScheduler().AddJob(job);
}

void ProcessingState::HandleDiscovery(const GeneralParams& params)
{
	string slaveID = StringConverter::Convert(params.GetValue("Slave ID"));
	string executorID = StringConverter::Convert(params.GetValue("Executor ID"));
	ClusterManager::Instace().GetScheduler().AddExecutor(slaveID, executorID);
}
