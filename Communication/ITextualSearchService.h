#pragma once

class GeneralParams;

class ITextualSearchService
{
public:
	virtual ~ITextualSearchService(){}
   virtual void Init(const GeneralParams& params) = 0;
   virtual void IndexDocument(const GeneralParams& params) = 0;
   virtual void Terminate() = 0;
};
