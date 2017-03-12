#pragma once

#include "State.h"

class ProcessingState : public State
{
public:
    ProcessingState(){}
    virtual ~ProcessingState(){}
	//The current state handling entry point, will address an appropriate response 
	//according to a received command type. if the recieved command is not supported 
	//in the current state an exception will be produced.
    void HandleState(StateContext& stateContext, CommandType commandType, const GeneralParams& params) override;
    
private:
	//The handling routine to the processing command type of the given state.
	//will create a new job and add it to the scheduler jobs queue.
    void HandleProcess(const GeneralParams& params);
};
