#include "TextualSearchServiceImpl.h"
#include "Core/Logger.h"
#include "ClusterManager/CommandType.h"
#include "GeneralParams.h"
#include "ITextualSearchService.h"

using namespace grpc;

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

Status TextualSearchServiceImpl::Terminate(ServerContext* context, const TextualSearchService::Empty* request, TextualSearchService::Empty* response)
{
	m_service.Terminate();
	return Status::OK;
}
