#include "GrpcServer.h"
#include <functional>
#include <grpc++/impl/codegen/service_type.h>
#include "Core/src/Assert.h"
#include "Core/src/Exception.h"
#include "Service.h"
#include "AsyncService.h"

using namespace std;
using namespace core;

GrpcServer::GrpcServer(const string& listeningPointAddress):
	m_completionQueueThread("CompletionQueue Thread", bind(&GrpcServer::CompletionQueueWorker, this))
{
	m_running = false;
	m_builder.AddListeningPort(listeningPointAddress, grpc::InsecureServerCredentials());
	m_completionQueue = m_builder.AddCompletionQueue();
	m_completionQueueThread.Start();
}

GrpcServer::~GrpcServer()
{
	m_server->Shutdown();
	m_completionQueue->Shutdown();
	m_completionQueueThread.Join();
}

void GrpcServer::AddService(const shared_ptr<Service>& service)
{
	ASSERT(m_running == false);
	m_builder.RegisterService(service->GetService());
	m_syncServices.emplace_back(service);
}

void GrpcServer::AddAsyncService(const shared_ptr<AsyncService>& service){
	ASSERT(m_running == false);
	m_builder.RegisterService(service->GetService());
	m_asyncServices.AddValue(service->GetService(), service);
}

shared_ptr<AsyncService> GrpcServer::GetAsyncService(void* tag){
	if(m_asyncServices.ContainsKey(tag) == true)
		return m_asyncServices[tag];
	else
		throw Exception(SOURCE, "No async service matches the supplied key - %ld", (long)tag);
}

void GrpcServer::Start()
{
	ASSERT(m_running.exchange(true) == false);
	m_server = m_builder.BuildAndStart();

	vector<void*> keys = m_asyncServices.GetAllKeys();
	for(auto& key : keys){
		const shared_ptr<AsyncService>& service = m_asyncServices[key];
		service->Connect(service.get());
	}
}

void GrpcServer::CompletionQueueWorker() {
	bool ok;
	void* tag;
    while(m_completionQueue->Next(&tag, &ok)){
		GPR_CODEGEN_ASSERT(ok);
		if(m_asyncServices.ContainsKey(tag)){
			shared_ptr<AsyncService>& service = m_asyncServices[tag];
			if(service->GetState() == AsyncService::State::NotConnected)
				service->Invoke(tag);
			else if(service->GetState() == AsyncService::State::Completed)
				service->Connect(service.get());
		}
	}
}
