#pragma once

#include <vector>
#include <utility>
#include <string>
#include "WordsTrieTree.h"
#include "Document.h"

class Document;
struct RedisModuleCtx;

class DocumentsIndexer
{
public:
	using DocumentWordsCount = std::vector<std::pair<std::string, int>>;
	static DocumentsIndexer& Instance();
	~DocumentsIndexer(){}
	void AddDocument(RedisModuleCtx* ctx, const Document& document);
	std::vector<Document> GetTopKDocumentsPerWord(RedisModuleCtx* ctx, const std::string& word, int k);

private:
	void AddDocumentMetaData(RedisModuleCtx* ctx, const Document& document);
	DocumentsIndexer(){}
private:
	WordsTrieTree m_IndexingStructure;
};
