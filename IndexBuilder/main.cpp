#include <memory>
#include <Core/Enviorment.h>
#include "Core/Logger.h"
#include "Core/CommandLine.h"
#include "Core/Logger.h"
#include "Core/TraceListener.h"
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
	IndexBuilder::Instance().InitializeMesos();
	IndexBuilder::Instance().WaitForCompletion();
	Logger::Instance().Flush();
    return 0;
}
