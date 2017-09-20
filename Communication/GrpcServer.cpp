#include "GrpcServer.h"
#include <functional>
#include <grpc++/impl/codegen/service_type.h>
#include "Core/Assert.h"
#include "Service.h"

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

void GrpcServer::AddAsyncService(const shared_ptr<Service>& service){
	ASSERT(m_running == false);
	m_builder.RegisterService(service->GetService());
	m_asyncServices.AddValue(service->GetService(), service);
}

void GrpcServer::Start()
{
	ASSERT(m_running.exchange(true) == false);
	m_server = m_builder.BuildAndStart();
}

void GrpcServer::CompletionQueueWorker() {
	bool ok;
	void* tag;
    while(m_completionQueue->Next(&tag, &ok)){
		GPR_CODEGEN_ASSERT(ok);
		if(m_asyncServices.ContainsKey(tag)){
			shared_ptr<Service>& service = m_asyncServices[tag];
			if(service->GetState() == Service::State::NotCompleted)
				service->SetConnected();
			else if(service->GetState() == Service::State::Completed)
				m_asyncServices.RemoveValue(tag);
		}
	}
}
