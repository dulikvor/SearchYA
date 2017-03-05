#include "PowerUpState.h"
#include <memory>
#include "Core/Exception.h"
#include "ConfigParams.h"
#include "ClusterManager.h"
#include "ProcessingState.h"

using namespace std;

void PowerUpState::HandleState(StateContext &stateContext, CommandType commandType, const Params &params)
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
        defualt:
        {
            throw core::Exception(SOURCE, "Unauthorized command was received - %s", commandType.ToString().c_str());
        }
    }
}

void PowerUpState::HandleInit(const Params& params)
{
	ConfigParams::Instance().Load(params);
	ClusterManager::Instace().Init();
}

