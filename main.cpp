#include "Core/Logger.h"
#include "Core/Trace.h"
#include "Core/LogDefs.h"
#include "ClusterManager/Params.h"
#include "ClusterManager/ClusterManager.h"
#include <list>

int main()
{
    Logger::Instance().Start("");
    ClusterManager::Instace();
    Trace(SOURCE, "Cluster Manager initiated");
	GeneralParam param;
	param.Set(std::list<std::string>({"a", "b"}));
	std::list<std::string> i = param;
	Trace(SOURCE, "Param value - %s", i.back().c_str());
    ClusterManager::Instace().WaitForCompletion();
    return 0;
}
