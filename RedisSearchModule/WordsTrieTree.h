#pragma once

#include <string>
#include "WordsTrieTreeNode.h"

class WordsTrieTree
{
public:
	WordsTrieTree():m_root('?'){}
	void AddWord(const std::string& documentName, const std::string& word, int wordAppearanceCount);
	WordsTrieTreeNode* const GetWordTreeNode(const std::string& word);

private:
	WordsTrieTreeNode m_root;
};
