#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Document
{
public:
	explicit Document(const std::string& name, const std::string& path):m_name(name), m_path(path){}
	Document(const std::string& name, const std::string& path, std::vector<std::pair<std::string, int>>& words);
	Document(const Document& obj) = delete;
	Document& operator=(const Document& obj) = delete;
	Document(const Document&& obj);
	Document& operator=(const Document&& obj);

	void AddWord(const std::string& word);
	//Properties
	const std::string& GetName() const {return m_name;}
	const std::unordered_map<std::string, int>& GetWords() const {return m_words;}
	std::string& GetName() {return m_name;}
	const std::string& GetPath() const {return m_path;}
	std::unordered_map<std::string, int>& GetWords() {return m_words;}

	static std::string Serialize(const Document& document);
	static Document Deserialize(const char* data, int length);

private:
	std::string m_name;
	std::string m_path;
	std::unordered_map<std::string, int> m_words;
};
