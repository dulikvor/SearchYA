#pragma once

#include "State.h"

class PowerUpState : public State
{
public:
    PowerUpState(){}
    virtual ~PowerUpState(){}

    void HandleState(StateContext& stateContext, CommandType commandType, const Params& params) override;
    
private:
    void HandleLoadParams(const Params& params);
};
