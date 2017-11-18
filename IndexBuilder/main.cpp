#include <memory>
#include "Core/src/Enviorment.h"
#include "Core/src/Logger.h"
#include "Core/src/CommandLine.h"
#include "Core/src/Logger.h"
#include "Core/src/TraceListener.h"
#include "cppkin/cppkin.h"
#include "IndexBuilder.h"

using namespace core;
using namespace std;

int main(int argc, char** argv)
{
	CommandLine::Instance().Parse(argc, const_cast<const char**>(argv));
	const string& workinDir = CommandLine::Instance().GetArgument("workingdir");
    Enviorment::Instance().Init();
	Logger::Instance().AddListener(make_shared<FileRotationListener>(TraceSeverity::Info, workinDir + "/IndexBuilder", 50 * 1024 * 1024, 20));
	Logger::Instance().Start(TraceSeverity::Info);
	TRACE_INFO("Index Builder starting");

	cppkin::GeneralParams cppkinParams;
	cppkinParams.AddParam(cppkin::ConfigTags::HOST_ADDRESS, string("127.0.0.1"));//ConfigParams::Instance().GetZipkinHostAddress());
	cppkinParams.AddParam(cppkin::ConfigTags::PORT, 9410);
	cppkinParams.AddParam(cppkin::ConfigTags::SERVICE_NAME, string("Index_Builder"));
	INIT(cppkinParams);
	IndexBuilder::Instance().InitializeMesos();
	IndexBuilder::Instance().WaitForCompletion();
	Logger::Instance().Flush();
    return 0;
}
