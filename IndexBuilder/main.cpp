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
	Enviorment::Instance().Init();
	Logger::Instance().AddListener(make_shared<FileRotationListener>(TraceSeverity::Info, 
		string("/home/dudilikvornik/Projects/MesosBenchMark/IndexBuilder/bin/IndexBuilder"), 
		50 * 1024 * 1024, 20));
    Logger::Instance().Start(TraceSeverity::Verbose);
	TRACE_INFO("Index Builder starting");
	IndexBuilder::Instance().InitializeMesos();
	IndexBuilder::Instance().WaitForCompletion();
	Logger::Instance().Flush();
    return 0;
}
