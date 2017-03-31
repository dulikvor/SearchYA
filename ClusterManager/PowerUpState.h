#pragma once

#include "State.h"

class PowerUpState : public State
{
public:
    PowerUpState(){}
    virtual ~PowerUpState(){}
	//The current state handling entry point, will address an appropriate response 
	//according to a received command type. if the recieved command is not supported 
	//in the current state an exception will be produced.
    void HandleState(StateContext& stateContext, CommandType commandType, const GeneralParams& params) override;
    
private:
	//The response to init command, will delegate the work to the cluster manager
	//init routine.
    void HandleInit(const GeneralParams& params);
    void HandleInitAck(const GeneralParams& params);
};
