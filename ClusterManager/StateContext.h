#pragma once

#include <memory>
#include "State.h"
#include "CommandType.h"
//Implements a state machine. the defualt property of the machine is the current
//state and the capability to react to a given action and to transit to a next state.
class StateContext
{
public:
    StateContext();
    ~StateContext(){delete m_currentState;}

    void HandleState(CommandType commandType, const GeneralParams& params){m_currentState->HandleState(*this, commandType, params);}
    void SetState(std::unique_ptr<State> newState){delete m_currentState; m_currentState = newState.release();}

private:
    State* m_currentState;
};
