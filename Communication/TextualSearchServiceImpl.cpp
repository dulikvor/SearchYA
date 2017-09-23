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
    m_application.Init(nativeParams);
	return Status::OK;
}


Status TextualSearchServiceImpl::IndexDocument(ServerContext* context, const TextualSearchService::Empty* request, TextualSearchService::Empty* response)
{
    m_application.IndexDocument(GeneralParams());
	return Status::OK;
}

Status TextualSearchServiceImpl::Terminate(ServerContext* context, const TextualSearchService::Empty* request, TextualSearchService::Empty* response)
{
	m_application.Terminate();
	return Status::OK;
}
