#pragma once

#include <string>

class WordsTrieTreeNode;

class WordsTrieTree
{
public:
	void AddWord(const std::string& documentName, const std::string& word, int wordAppearanceCount);
	WordsTrieTreeNode* const GetWordTreeNode(const std::string& word);

private:
	WordsTrieTreeNode* m_root;
};
