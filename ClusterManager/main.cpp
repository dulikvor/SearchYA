#include "Logger/Logger.h"
#include "Logger/Trace.h"
#include "Logger/LogDefs.h"
#include "Params/Params.h"
#include "Application/ClusterManager.h"

int main()
{
    Logger::Instance().Start("");
    ClusterManager::Instace();
    Trace(SOURCE, "Cluster Manager initiated");
    ClusterManager::Instace().WaitForCompletion();
    return 0;
}
