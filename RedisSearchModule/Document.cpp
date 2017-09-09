#include "Document.h"
#include "Communication/Serializor.h"

using namespace std;

Document::Document(const string& name, const string& path, vector<pair<string, int>>& words)
	:m_name(name), m_path(path)
{
	for(auto& vecPair : words)
	{
		m_words[vecPair.first] = vecPair.second;
	}
}


Document::Document(const Document&& obj)
{
	m_name = move(obj.GetName());
	m_path = move(obj.GetPath());
	m_words = move(obj.GetWords());
}

Document& Document::operator=(const Document&& obj)
{
	m_name = move(obj.GetName());
	m_path = move(obj.GetPath());
	m_words = move(obj.GetWords());
	return *this;
}

void Document::AddWord(const std::string &word) {
    if(m_words.find(word) == m_words.end())
        m_words[word] = 1;
    else
        m_words[word]++;
}

string Document::Serialize(const Document& document)
{
	Serializor serializeContext;
	Serializor::Serialize(serializeContext, document.GetName());
	Serializor::Serialize(serializeContext, document.GetPath());
	Serializor::Serialize(serializeContext, (int)document.GetWords().size());
	for(auto& hashPair : document.GetWords())
	{
		Serializor::Serialize(serializeContext, hashPair.first);	
		Serializor::Serialize(serializeContext, hashPair.second);	
	}
	return serializeContext.GetBuffer();
}

Document Document::Deserialize(const char *data, int length)
{
	Serializor serializeContext(data, length);
	string name = Serializor::DeserializeString(serializeContext);
	string path = Serializor::DeserializeString(serializeContext);
	int wordsCount = Serializor::DeserializeInt(serializeContext);
	vector<pair<string, int>> words;
	words.reserve(wordsCount);
	for(int index = 0; index < wordsCount; index++)
	{
        string word = Serializor::DeserializeString(serializeContext);
		int wordCount = Serializor::DeserializeInt(serializeContext);
		words.emplace_back(word, wordCount);
	}
	return Document(name, path, words);
}
