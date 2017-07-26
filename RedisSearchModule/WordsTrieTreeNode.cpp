#include "WordsTrieTreeNode.h"

using namespace std;

WordsTrieTreeNode::WordsTrieTreeNode(char letter):m_letter(letter)
{
	m_documentsRanking.reset(new DocumentRanking(&WordsTrieTreeNode::DocumentWordAppearCompare));
}

WordsTrieTreeNode::~WordsTrieTreeNode()
{
	for(auto& node : m_nodes)
	{
		delete node.second;
	}
}

WordsTrieTreeNode* WordsTrieTreeNode::AddNode(char letter)
{
	if(m_nodes.find(letter) == m_nodes.end())
	{
		m_nodes[letter] = new WordsTrieTreeNode(letter);
	}
	return m_nodes[letter];
}


void WordsTrieTreeNode::AddDocument(const WordCountInDoc& wordCountInDoc)
{
	m_documentsRanking->push(wordCountInDoc);
}

vector<WordsTrieTreeNode::WordCountInDoc> WordsTrieTreeNode::GetTopKDocuments(int k) {
	vector<WordCountInDoc> topDocuments;
	while(k-- && m_documentsRanking->size() > 0)
	{
		topDocuments.emplace_back(m_documentsRanking->top());
		m_documentsRanking->pop();
	}

	for(const WordsTrieTreeNode::WordCountInDoc& documentNameScore : topDocuments)
		m_documentsRanking->emplace(documentNameScore);

	return topDocuments;
}

WordsTrieTreeNode* const WordsTrieTreeNode::GetNode(char letter) const {

	auto it = m_nodes.find(letter);
	if(it != m_nodes.end())
		return it->second;
	else
		return nullptr;
}
