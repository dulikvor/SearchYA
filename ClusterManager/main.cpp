#include <memory>
#include <string>
#include "Core/Logger.h"
#include "Core/TraceListener.h"
#include "ClusterManager/CommandType.h"
#include "Communication/GeneralParams.h"
#include "Communication/Param.h"
#include "ClusterManager/ClusterManager.h"
#include "Communication/GeneretedFiles/ClusterService.pb.h"
#include "Core/TimedAsyncExecutor.h"
#include "Core/AsyncTask.h"
#include "Core/Enviorment.h"

using namespace core;
using namespace std;

ClusterService::Params CreateStubConfig();

int main()
{
	Enviorment::Instance().Init();
	Logger::Instance().AddListener(make_shared<FileRotationListener>(TraceSeverity::Info, string("Service"), 50 * 1024 * 1024, 20));
    Logger::Instance().Start(TraceSeverity::Info);
	TRACE_INFO("MesosBenchMark starting");
	ClusterManager::Instace().InitializeServer("127.0.0.1:50051");
   	ClusterManager::Instace().WaitForCompletion();
	Logger::Instance().Flush();
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
	param->set_valuestring("~");
	param->set_type(ClusterService::Param::Primitive);
	param->set_name("Execution Directory");
	return params;
}
