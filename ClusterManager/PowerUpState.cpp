#include "PowerUpState.h"
#include <memory>
#include "Core/LogDefs.h"
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
        defualt:
        {
            throw Core::Exception(SOURCE, "Unauthorized command was received - %s", commandType.ToString().c_str());
        }
    }
}

void PowerUpState::HandleInit(const Params& params)
{
	ConfigParams::Instance().Load(params);
	ClusterManager::Instace().Init();
}
