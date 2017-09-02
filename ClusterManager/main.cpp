#include <memory>
#include <string>
#include "Core/Logger.h"
#include "Core/TraceListener.h"
#include "ClusterManager/CommandType.h"
#include "Communication/GeneralParams.h"
#include "Communication/Param.h"
#include "ClusterManager/ClusterManager.h"
#include "Communication/GeneretedFiles/TextualSearchService.pb.h"
#include "Core/TimedAsyncExecutor.h"
#include "Core/AsyncTask.h"
#include "Core/Enviorment.h"
#include "Core/CommandLine.h"

using namespace core;
using namespace std;

TextualSearchService::Params CreateStubConfig();

int main(int argc, char** argv)
{
	CommandLine::Instance().Parse(argc, (const char**)argv);
	Enviorment::Instance().Init();
	string workingDir = Enviorment::Instance().GetProcessPath() + "/" + CommandLine::Instance().GetArgument("workingdir");
	Logger::Instance().AddListener(make_shared<FileRotationListener>(TraceSeverity::Info, workingDir + "/Service", 50 * 1024 * 1024, 20));
    Logger::Instance().Start(TraceSeverity::Info);
	TRACE_INFO("MesosBenchMark starting");
	ClusterManager::Instace().InitializeServer("127.0.0.1:50051");
   	ClusterManager::Instace().WaitForCompletion();
	Logger::Instance().Flush();
    return 0;
}

TextualSearchService::Params CreateStubConfig()
{
	TextualSearchService::Params params;
	TextualSearchService::Param* param = params.add_genericparams();
	param->add_valuestringcollection("127.0.0.1");
	param->add_valuestringcollection("127.0.0.1");
	param->set_name("Processing units address");
	param->set_type(TextualSearchService::Param::StringCollection);
	param = params.add_genericparams();
	param->set_valueint(0);
	param->set_name("Recovery Time");
	param->set_type(TextualSearchService::Param::Primitive);
	param = params.add_genericparams();
	param->set_valuestring("127.0.0.1");
	param->set_name("Host Address");
	param->set_type(TextualSearchService::Param::Primitive);
	param = params.add_genericparams();
	param->set_valuestring("127.0.0.1");
	param->set_name("Mesos Host Address");
	param->set_type(TextualSearchService::Param::Primitive);
	param = params.add_genericparams();
	param->set_valuestring("~");
	param->set_type(TextualSearchService::Param::Primitive);
	param->set_name("Execution Directory");
	return params;
}
