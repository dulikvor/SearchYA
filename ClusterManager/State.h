#pragma once

#include "CommandType.h"

class Params;
class StateContext;

class State
{
public:
    State(){}
    virtual ~State(){}

    virtual void HandleState(StateContext& stateContext, CommandType commandType, const Params& params) = 0;

protected:
	void HandleTerminate();
};
