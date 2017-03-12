#pragma once

#include "State.h"

class PowerUpState : public State
{
public:
    PowerUpState(){}
    virtual ~PowerUpState(){}

    void HandleState(StateContext& stateContext, CommandType commandType, const GeneralParams& params) override;
    
private:
    void HandleInit(const GeneralParams& params);
};
