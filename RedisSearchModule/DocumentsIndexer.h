#pragma once

#include <list>
#include <utility>
#include <string>
#include "WordsTrieTree.h"

class Document;

class DocumentsIndexer
{
public:
	using DocumentWordsCount = std::list<std::pair<std::string, int>>;
	static DocumentsIndexer& Instance();
	~DocumentsIndexer(){}
	void AddDocument(const Document& document);

private:
	DocumentsIndexer(){}
private:
	WordsTrieTree m_IndexingStructure;
};
