#pragma once

#include "GeneretedFiles/TextualSearchService.grpc.pb.h"

class ITextualSearchService;

class TextualSearchServiceImpl : public ::TextualSearchService::TextualSearch::Service
{
public:
	TextualSearchServiceImpl(ITextualSearchService& service): m_service(service){}
	virtual ~TextualSearchServiceImpl(){}
   ::grpc::Status Init(::grpc::ServerContext* context, const ::TextualSearchService::Params* request, ::TextualSearchService::Empty* response) override;
   ::grpc::Status IndexDocument(::grpc::ServerContext* context, const ::TextualSearchService::Empty* request, ::TextualSearchService::Empty* response) override;
	::grpc::Status GetTopKDocuments(::grpc::ServerContext* context, const ::TextualSearchService::TopKDocumentsPerWordRequest* request, ::TextualSearchService::TopKDocumentsReply* response) override;
   	::grpc::Status Terminate(::grpc::ServerContext* context, const ::TextualSearchService::Empty* request, ::TextualSearchService::Empty* response) override;

private:
	ITextualSearchService& m_service;
};
