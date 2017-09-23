#pragma once

#include <string>
#include <vector>

class Document;
class GeneralParams;

class ITextualSearchService
{
public:
	virtual ~ITextualSearchService(){}
    virtual void Init(const GeneralParams& params) = 0;
    virtual void IndexDocument(const GeneralParams& params) = 0;
    virtual void GetTopKDocuments(const std::string& word, int k, void* callbackTag) = 0;
    virtual void Terminate() = 0;
};
