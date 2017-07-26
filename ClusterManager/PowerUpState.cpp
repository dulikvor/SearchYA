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
			stateContext.SetState(unique_ptr<State>(new ProcessingState()));
            break;
        }
		case CommandType::Terminate:
		{
			State::HandleTerminate();
			break;
		}
		case CommandType::GetTopK:
		case CommandType::Index:
        defualt:
        {
            throw core::Exception(SOURCE, "Unauthorized command was received - %s", commandType.ToString().c_str());
        }
    }
}

void PowerUpState::HandleInit(const GeneralParams& params)
{
	ConfigParams::Instance().Load(params);
	ClusterManager::Instace().HandleInit();
}
