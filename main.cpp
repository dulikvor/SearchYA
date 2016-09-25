#include "Core/Logger.h"
#include "Core/Trace.h"
#include "Core/LogDefs.h"
#include "ClusterManager/CommandType.h"
#include "ClusterManager/Params.h"
#include "ClusterManager/Param.h"
#include "ClusterManager/ClusterManager.h"
#include "Communication/GeneretedFiles/ClusterService.pb.h"

ClusterService::Params CreateStubConfig();

int main()
{
    Logger::Instance().Start("");
	Params params;
	params.Load(CreateStubConfig());
    ClusterManager::Instace().NewCommand(CommandType::LoadParams, params);
    Trace(SOURCE, "Cluster Manager initiated");
    ClusterManager::Instace().WaitForCompletion();
    return 0;
}

ClusterService::Params CreateStubConfig()
{
	ClusterService::Params params;
	ClusterService::Param* param = params.add_genericparams();
	param->add_valuestringcollection("127.0.0.1");
	param->add_valuestringcollection("127.0.0.1");
	param->set_name("Processing units address");
	param->set_type(ClusterService::Param::StringCollection);
	param = params.add_genericparams();
	param->set_valueint(0);
	param->set_name("Recovery Time");
	param->set_type(ClusterService::Param::Primitive);
	param = params.add_genericparams();
	param->set_valuestring("127.0.0.1");
	param->set_name("Host Address");
	param->set_type(ClusterService::Param::Primitive);
	param = params.add_genericparams();
	param->set_valuestring("127.0.0.1");
	param->set_name("Mesos Host Address");
	param->set_type(ClusterService::Param::Primitive);
	param = params.add_genericparams();
	param->set_valueint(0);
	param->set_name("Mesos Port Address");
	param->set_type(ClusterService::Param::Primitive);
	param = params.add_genericparams();
	param->set_valuestring("~");
	param->set_type(ClusterService::Param::Primitive);
	param->set_name("Execution Directory");
	return params;
}
