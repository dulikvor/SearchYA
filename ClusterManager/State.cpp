#include "State.h"
#include "ClusterManager.h"

void State::HandleTerminate()
{
	ClusterManager::Instace().HandleTerminate();
}

