#include "WordsTrieTree.h"
#include "Core/Exception.h"

using namespace std;
using namespace core;

void WordsTrieTree::AddWord(const string& documentName, const string& word, int wordAppearanceCount)
{
	string::const_iterator it = word.begin();
	WordsTrieTreeNode* current = &m_root;
	while(it != word.end())
	{
		current = current->AddNode(*it);
		it++;
	}
	current->AddDocument(make_pair(documentName, wordAppearanceCount));
}

WordsTrieTreeNode* const WordsTrieTree::GetWordTreeNode(const std::string &word) {
	string::const_iterator it = word.begin();
	WordsTrieTreeNode* current = &m_root;
	while(it != word.end() && current != nullptr)
	{
		current = current->GetNode(*it);
	}
	if(current == nullptr)
		throw Exception(SOURCE, "Requested word %s was not found", word.c_str());
	return current;
}
