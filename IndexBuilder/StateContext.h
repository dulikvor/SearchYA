#pragma once

#include <memory>
#include "State.h"
#include "CommandType.h"

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
