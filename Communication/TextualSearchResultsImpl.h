#pragma once

#include <memory>
#include <string>
#include <vector>
#include "grpc++/grpc++.h"
#include "GeneretedFiles/TextualSearchResultsService.grpc.pb.h"
#include "Service.h"

class GrpcServer;

class TextualSearchResultsImpl : public Service
{
public:
    TextualSearchResultsImpl(grpc::ServerCompletionQueue& completionQueue);
    virtual ~TextualSearchResultsImpl(){}
    void AsyncRequestGetTopKDocuments(const std::string& word, int k);
    void ProvideResult(const std::vector<std::string>& documentsNames);
private:
    using TextualSearchResultsAsyncService =::TextualSearchResultsService::TextualSearchResults::AsyncService;
    using Responder = ::grpc::ServerAsyncResponseWriter<::TextualSearchResultsService::TopKDocumentsReply>;
    grpc::ServerContext m_serverContex;
    std::unique_ptr<Responder> m_responder;
    grpc::ServerCompletionQueue& m_completionQueue;
};
