#pragma once

#include <unordered_map>
#include <queue>
#include <utility>
#include <vector>
#include <functional>
#include <memory>

class WordsTrieTreeNode
{
public:
	using WordCountInDoc = std::pair<std::string, int>;

public:
	explicit WordsTrieTreeNode(char letter);
	~WordsTrieTreeNode();
	WordsTrieTreeNode* AddNode(char letter);
	void AddDocument(const WordCountInDoc& wordCountInDoc);

private:
	static bool DocumentWordAppearCompare(const WordCountInDoc& obj1, const WordCountInDoc& obj2) { return obj1.second < obj2.second; }

private:
	using DocumentRanking = std::priority_queue<WordCountInDoc, std::vector<WordCountInDoc>, std::function<bool(const WordCountInDoc&, const WordCountInDoc&)>>;
	std::unordered_map<char, WordsTrieTreeNode*> m_nodes;
	char m_letter;
	std::unique_ptr<DocumentRanking> m_documentsRanking; 


};
