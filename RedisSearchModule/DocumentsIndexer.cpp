#include "DocumentsIndexer.h"
#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Redis/redismodule.h"

using namespace std;
using namespace core;

DocumentsIndexer& DocumentsIndexer::Instance()
{
	static DocumentsIndexer instance;
	return instance;
}

vector<Document> DocumentsIndexer::GetTopKDocumentsPerWord(RedisModuleCtx* ctx, const string &word, int k) {
	TRACE_INFO("{Word - %s, k - %d}, search for top documents", word.c_str(), k);
	WordsTrieTreeNode* node = m_IndexingStructure.GetWordTreeNode(word);
	VERIFY(node != nullptr, "Requested word was not found - %s", word.c_str());
	vector<WordsTrieTreeNode::WordCountInDoc> wordTopDocuments = node->GetTopKDocuments(k);
	vector<Document> topDocuments;
	for(const auto& documentNameScore : wordTopDocuments)
	{
		TRACE_INFO("{Document - %s} - score - %d", documentNameScore.first.c_str(), documentNameScore.second);
		RedisModuleCallReply* reply = RedisModule_Call(ctx, "HGET", "cc", "DocumentsMetaData", documentNameScore.first.c_str());
		size_t messageLength = 0;
		const char* hgetResult = RedisModule_CallReplyStringPtr(reply,&messageLength);
		VERIFY(RedisModule_CallReplyType(reply) != REDISMODULE_REPLY_ERROR && messageLength > 0, "Couldn't get document %s meta data - %s",
			   documentNameScore.first.c_str(), messageLength == 0 ? "Nil was returned" : hgetResult);
		topDocuments.emplace_back(documentNameScore.first, string(hgetResult, messageLength));
		RedisModule_FreeCallReply(reply);
	}
	return topDocuments;
}

void DocumentsIndexer::AddDocument(RedisModuleCtx* ctx, const Document& document)
{
	AddDocumentMetaData(ctx, document);
	for(auto& wordCount : document.GetWords())
	{
		//first - word, second - word appearance count
		m_IndexingStructure.AddWord(document.GetName(), wordCount.first, wordCount.second);
	}
}

void DocumentsIndexer::AddDocumentMetaData(RedisModuleCtx* ctx, const Document &document) {
	TRACE_INFO("{Document - %s} - Adding meta data: path - %s", document.GetName().c_str(), document.GetPath().c_str());
	RedisModuleCallReply* reply = RedisModule_Call(ctx, "HSET", "ccc", "DocumentsMetaData",
	document.GetName().c_str(), document.GetPath().c_str());
	size_t errMessageLength;
	VERIFY(RedisModule_CallReplyType(reply) != REDISMODULE_REPLY_ERROR, "Couldn't set the has - %s", RedisModule_CallReplyStringPtr(reply,&errMessageLength));
	RedisModule_FreeCallReply(reply);
}
