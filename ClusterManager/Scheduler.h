#pragma once

#include <string>
#include <unordered_map>
#include "Core/OrderedList.h"

class Params;
class Pu;

class Scheduler
{
public:
	Scheduler(){}
	~Scheduler(){}
	//Initialize will address the config data reading the cluster information, the different processing units locations
	//indicated by thier node address (they addresses may return). the function will create an internal object representing
	//each processing unit, storing each process by its location address, no need to sort the most busy processes, since they are all idle
	//at the moment.
	void Initialize();

private:
	std::unordered_map<NodeAddress, OrderedList<Pu>> m_processingUnits;
}
