#pragma once

#include "CommandType.h"

class GeneralParams;
class StateContext;

class State
{
public:
    State(){}
    virtual ~State(){}

    virtual void HandleState(StateContext& stateContext, CommandType commandType, const GeneralParams& params) = 0;

protected:
	void HandleTerminate();
};
