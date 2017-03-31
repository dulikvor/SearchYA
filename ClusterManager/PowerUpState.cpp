#include "PowerUpState.h"
#include <memory>
#include "Core/Exception.h"
#include "Communication/GeneralParams.h"
#include "ConfigParams.h"
#include "ClusterManager.h"
#include "ProcessingState.h"

using namespace std;

void PowerUpState::HandleState(StateContext &stateContext, CommandType commandType, const GeneralParams &params)
{
    switch(commandType)
    {
		case CommandType::Init:
        {
            HandleInit(params);
            break;
        }
		case CommandType::InitAck:
		{
            HandleInitAck(params);
			stateContext.SetState(unique_ptr<State>(new ProcessingState()));
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

void PowerUpState::HandleInit(const GeneralParams& params)
{
	ConfigParams::Instance().Load(params);
	ClusterManager::Instace().Init();
}

void PowerUpState::HandleInitAck(const GeneralParams& params)
{
	string slaveID = StringConverter::Convert(params.GetValue("Slave ID"));
	string executorID = StringConverter::Convert(params.GetValue("Executor ID"));
	ClusterManager::Instace().GetScheduler().AddExecutor(slaveID, executorID);
}
