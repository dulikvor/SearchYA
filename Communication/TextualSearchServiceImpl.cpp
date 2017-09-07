#include "TextualSearchServiceImpl.h"
#include "Core/Logger.h"
#include "ClusterManager/CommandType.h"
#include "RedisSearchModule/Document.h"
#include "GeneralParams.h"
#include "ITextualSearchService.h"

using namespace grpc;
using namespace std;

Status TextualSearchServiceImpl::Init(ServerContext* context, const ::TextualSearchService::Params* request, ::TextualSearchService::Empty* response)

{
    GeneralParams nativeParams;
    nativeParams.Load(*request);
    m_service.Init(nativeParams);
	return Status::OK;
}


Status TextualSearchServiceImpl::IndexDocument(ServerContext* context, const TextualSearchService::Empty* request, TextualSearchService::Empty* response)
{
    m_service.IndexDocument(GeneralParams());
	return Status::OK;
}

Status TextualSearchServiceImpl::GetTopKDocuments(ServerContext* context, const TextualSearchService::TopKDocumentsPerWordRequest* request, TextualSearchService::TopKDocumentsReply* response)
{
    vector<Document> topKDocuments = m_service.GetTopKDocuments("Hello", 1);//request->word(), request->k());
    for(const Document& doc : topKDocuments) {
        response->add_documentname(doc.GetName());
    }
    return Status::OK;
}

Status TextualSearchServiceImpl::Terminate(ServerContext* context, const TextualSearchService::Empty* request, TextualSearchService::Empty* response)
{
	m_service.Terminate();
	return Status::OK;
}
