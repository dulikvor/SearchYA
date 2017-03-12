#include "StateContext.h"
#include "PowerUpState.h"
#include "ProcessingState.h"

StateContext::StateContext()
    :m_currentState(new ProcessingState())
{
}
