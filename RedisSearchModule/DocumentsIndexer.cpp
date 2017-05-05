#include "DocumentsIndexer.h"
#include "Document.h"

DocumentsIndexer& DocumentsIndexer::Instance()
{
	static DocumentsIndexer instance;
	return instance;
}


void DocumentsIndexer::AddDocument(const Document& document)
{
	for(auto& wordCount : document.GetWords())
	{
		//first - word, second - word appearance count
		m_IndexingStructure.AddWord(document.GetName(), wordCount.first, wordCount.second);
	}
}
