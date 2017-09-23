#pragma once

#include <memory>
#include <string>
#include <vector>
#include "grpc++/grpc++.h"
#include "GeneretedFiles/TextualSearchResultsService.grpc.pb.h"
#include "AsyncService.h"

class GrpcServer;
class ITextualSearchService;

class TextualSearchResultsImpl : public AsyncService
{
public:
    TextualSearchResultsImpl(grpc::ServerCompletionQueue& completionQueue, ITextualSearchService& consumer);
    virtual ~TextualSearchResultsImpl(){}
    void ProvideResult(const std::vector<std::string>& documentsNames);

private:
    void Connect(void* tag) override;
    void Invoke(void* tag) override;

private:
    using TextualSearchResultsAsyncService =::TextualSearchResultsService::TextualSearchResults::AsyncService;
    using Responder = ::grpc::ServerAsyncResponseWriter<::TextualSearchResultsService::TopKDocumentsReply>;
    grpc::ServerContext m_serverContex;
    std::unique_ptr<Responder> m_responder;
    ::TextualSearchResultsService::TopKDocumentsPerWordRequest m_request;
    grpc::ServerCompletionQueue& m_completionQueue;
    ITextualSearchService& m_consumer;

};
