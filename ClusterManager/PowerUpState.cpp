#include "PowerUpState.h"
#include "Core/LogDefs.h"
#include "Core/Exception.h"

void PowerUpState::HandleState(StateContext &stateContext, CommandType commandType, const Params &params)
{
    switch(commandType)
    {
        case CommandType::LoadParams:
        {
            HandleLoadParams(params);
            break;
        }
        defualt:
        {
            throw Exception(SOURCE, "Unauthorized command was received - %d", commandType);
        }
    }
}

void PowerUpState::HandleLoadParams(const Params& params)
{

}
