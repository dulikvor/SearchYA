#include "SearchModule.h"
#include <string>
#include <memory>
#include <iostream>
#include "Core/Enviorment.h"
#include "Core/CommandLine.h"
#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Core/TraceListener.h"
#include "DocumentsIndexer.h"
#include "Document.h"

using namespace std;
using namespace core;

extern "C"
{
	int AddDocument(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
	{
		VERIFY(argc == 1, "AddDocument arg list cannot be larger than 1");
		size_t stringLength = 0;
		const char* binaryData = RedisModule_StringPtrLen(argv[0], &stringLength);
		Document receivedDocument = std::move(Document::Deserialize(binaryData, (int)stringLength));
		DocumentsIndexer::Instance().AddDocument(receivedDocument);
		RedisModule_ReplyWithSimpleString(ctx,"OK");
		return REDISMODULE_OK;
	}

	int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
	{

		if (RedisModule_Init(ctx, "Search", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR)
			    return REDISMODULE_ERR;
		VERIFY(argc == 1, "AddDocument arg list cannot be larger than 1");
		size_t stringLength;
		const char* binaryData = RedisModule_StringPtrLen(argv[0], &stringLength);
		CommandLine commandLine(argc, &binaryData);

		Enviorment::Instance().Init();
		string workingDir = Enviorment::Instance().GetWorkingDir() + "/" + commandLine.GetArgument("workingdir");
		Logger::Instance().AddListener(make_shared<FileRotationListener>(TraceSeverity::Info, workingDir + "/SearchModule", 50 * 1024 * 1024, 20));
		Logger::Instance().Start(TraceSeverity::Info);

		if(RedisModule_CreateCommand(ctx, "Search.AddDocument", &AddDocument, "", 0, 0, 0) == REDISMODULE_ERR)
				return REDISMODULE_ERR;

		TRACE_INFO("Redis Search Module is up");
		return REDISMODULE_OK;
	}
}

