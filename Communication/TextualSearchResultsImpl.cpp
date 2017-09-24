#include "TextualSearchResultsImpl.h"
#include "ITextualSearchService.h"

using namespace std;

TextualSearchResultsImpl::TextualSearchResultsImpl(grpc::ServerCompletionQueue& completionQueue, ITextualSearchService& consumer):
    m_completionQueue(completionQueue), m_consumer(consumer){
    m_service = shared_ptr<grpc::Service>(new TextualSearchResultsAsyncService());
    m_responder.reset(new Responder(&m_serverContex));
}

void TextualSearchResultsImpl::Connect(void* tag) {
    TextualSearchResultsAsyncService& service = static_cast<TextualSearchResultsAsyncService&>(*m_service);
    service.RequestGetTopKDocuments(&m_serverContex, &m_request, m_responder.get(), &m_completionQueue,
        &m_completionQueue, tag);
    m_state = AsyncService::State ::NotConnected;
}

void TextualSearchResultsImpl::Invoke(void *tag) {
    m_state = AsyncService::State::Connected;
    int k = m_request.k();
    string word = m_request.word();
    m_consumer.GetTopKDocuments(word, k, this);
}


void TextualSearchResultsImpl::ProvideResult(const vector<string>& documentsNames){
    ::TextualSearchResultsService::TopKDocumentsReply reply;
    for(const string& documentName : documentsNames)
        reply.add_documentname(documentName);
    m_responder->Finish(reply, grpc::Status::OK, this);
    m_state = AsyncService::State::Completed;
}
