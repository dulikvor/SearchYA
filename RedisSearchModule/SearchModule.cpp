#include "SearchModule.h"
#include <string>
#include <memory>
#include <iostream>
#include "Core/Enviorment.h"
#include "Core/CommandLine.h"
#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Core/TraceListener.h"
#include "Core/Deleter.h"
#include "Communication/Serializor.h"
#include "DocumentsIndexer.h"
#define REDIS_EXTERN
#include "Redis/redismodule.h"

using namespace std;
using namespace core;

extern "C"
{
	int GetWordTopKDocument(RedisModuleCtx* ctx, RedisModuleString** argv, int argc)
	{
		//command name + word + k == 3
		VERIFY(argc == 3, "GetWordTopKDocument arg list cannot be larger than 2");
		size_t binaryDataLength = 0;
		const char* word = RedisModule_StringPtrLen(argv[1], &binaryDataLength);

		const char* k = RedisModule_StringPtrLen(argv[2], &binaryDataLength);
        Serializor serializor(k, binaryDataLength);
		vector<Document> topKDocuments = DocumentsIndexer::Instance().GetTopKDocumentsPerWord(ctx, string(word),
             Serializor::DeserializeInt(serializor));

		RedisModule_ReplyWithArray(ctx, REDISMODULE_POSTPONED_ARRAY_LEN);
		for(const auto& document : topKDocuments)
		{
			string serializedDocument = Document::Serialize(document);
			RedisModule_ReplyWithStringBuffer(ctx, serializedDocument.c_str(), serializedDocument.length());
		}
		RedisModule_ReplySetArrayLength(ctx, topKDocuments.size());
		return REDISMODULE_OK;
	}

	int AddDocument(RedisModuleCtx *ctx, RedisModuleString **argv, int argc)
	{
        TRACE_INFO("Add Document Command was received");
		//command name + document serialized data == 2
		VERIFY(argc == 2, "AddDocument arg list cannot be larger than 2");
		size_t stringLength = 0;
		const char* binaryData = RedisModule_StringPtrLen(argv[1], &stringLength);
		Document receivedDocument = std::move(Document::Deserialize(binaryData, (int)stringLength));
		TRACE_INFO("Adding new document - %s", receivedDocument.GetName().c_str());
		DocumentsIndexer::Instance().AddDocument(ctx, receivedDocument);
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
		CommandLine::Instance().Parse(argc, &binaryData);

		Enviorment::Instance().Init();
		string workingDir = Enviorment::Instance().GetProcessPath() + "/" + CommandLine::Instance().GetArgument("workingdir");
		Logger::Instance().AddListener(make_shared<FileRotationListener>(TraceSeverity::Info, workingDir + "SearchModule", 50 * 1024 * 1024, 20));
		Logger::Instance().Start(TraceSeverity::Info);

		if(RedisModule_CreateCommand(ctx, "Search.AddDocument", &AddDocument, "", 0, 0, 0) == REDISMODULE_ERR)
				return REDISMODULE_ERR;


		TRACE_INFO("Redis Search Module is up");
		return REDISMODULE_OK;
	}
}

