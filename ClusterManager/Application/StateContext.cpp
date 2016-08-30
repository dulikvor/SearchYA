#include "StateContext.h"
#include "PowerUpState.h"

StateContext::StateContext()
    :m_currentState(new PowerUpState())
{
}
