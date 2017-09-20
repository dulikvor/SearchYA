//
// Created by dudilikvornik on 9/17/17.
//

#include "TextualSearchResultsImpl.h"

using namespace std;

TextualSearchResultsImpl::TextualSearchResultsImpl(grpc::ServerCompletionQueue& completionQueue):
    m_completionQueue(completionQueue){
    m_service = shared_ptr<grpc::Service>(new TextualSearchResultsAsyncService());
    m_responder.reset(new Responder(&m_serverContex));
}

void TextualSearchResultsImpl::AsyncRequestGetTopKDocuments(const std::string &word, int k) {
    unique_ptr<::TextualSearchResultsService::TopKDocumentsPerWordRequest> request(
            new ::TextualSearchResultsService::TopKDocumentsPerWordRequest());
    request->set_word(word);
    request->set_k(k);
    TextualSearchResultsAsyncService& service = static_cast<TextualSearchResultsAsyncService&>(*m_service);
    service.RequestGetTopKDocuments(&m_serverContex, request.get(), m_responder.get(), &m_completionQueue,
        &m_completionQueue, this);
}


void TextualSearchResultsImpl::ProvideResult(const vector<string>& documentsNames){
    m_connectedEvent.WaitOne();
    ::TextualSearchResultsService::TopKDocumentsReply reply;
    for(const string& documentName : documentsNames)
        reply.add_documentname(documentName);
    m_responder->Finish(reply, grpc::Status::OK, this);
    m_state = Service::State::Completed;
}
