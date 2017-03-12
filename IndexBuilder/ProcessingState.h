#pragma once

#include "State.h"

class ProcessingState : public State
{
public:
    ProcessingState(){}
    virtual ~ProcessingState(){}

    void HandleState(StateContext& stateContext, CommandType commandType, const GeneralParams& params) override;
    
private:
    void HandleProcess(const GeneralParams& params);
};
