#include "WordsTrieTree.h"
#include "WordsTrieTreeNode.h"

using namespace std;

void WordsTrieTree::AddWord(const string& documentName, const string& word, int wordAppearanceCount)
{
	string::const_iterator it = word.begin();
	WordsTrieTreeNode* current = m_root;
	while(it != word.end())
	{
		current = current->AddNode(*it);
		it++;
	}
	current->AddDocument(make_pair(documentName, wordAppearanceCount));
}
	
