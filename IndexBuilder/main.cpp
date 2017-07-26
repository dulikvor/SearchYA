#include <memory>
#include <string>
#include "Core/Logger.h"
#include "Core/TraceListener.h"
#include "Core/Enviorment.h"
#include "IndexBuilder.h"

using namespace core;
using namespace std;

int main()
{
	Logger::Instance().AddListener(make_shared<FileRotationListener>(TraceSeverity::Info,
		string("/home/dudilikvornik/Projects/SearchYA/IndexBuilder/bin/IndexBuilder"), 
		50 * 1024 * 1024, 20));
    Logger::Instance().Start(TraceSeverity::Verbose);
	Enviorment::Instance().Init();
	TRACE_INFO("Index Builder starting");
	IndexBuilder::Instance().InitializeMesos();
	IndexBuilder::Instance().WaitForCompletion();
	Logger::Instance().Flush();
    return 0;
}
