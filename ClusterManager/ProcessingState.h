#pragma once

#include "State.h"

class ProcessingState : public State
{
public:
    ProcessingState(){}
    virtual ~ProcessingState(){}

    void HandleState(StateContext& stateContext, CommandType commandType, const Params& params) override;
    
private:
    void HandleProcess(const Params& params);
};
