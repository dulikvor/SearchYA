#include "ProcessingState.h"
#include "Core/Exception.h"
#include "Core/Logger.h"
#include "IndexBuilder.h"

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
		case CommandType::Terminate:
		{
			State::HandleTerminate();
			break;
		}
		case CommandType::Init:
		{
			//Due to race condition with received init and returned reply.
			break;
		}
		case CommandType::Failed:
        defualt:
        {
            throw core::Exception(SOURCE, "Unauthorized command was received - %s", commandType.ToString().c_str());
        }
    }
}

void ProcessingState::HandleProcess(const GeneralParams& params)
{
	IndexBuilder::Instance().Processing(params);
}
