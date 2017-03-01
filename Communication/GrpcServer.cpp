#include "GrpcServer.h"
#include <grpc++/impl/codegen/service_type.h>
#include "Core/Assert.h"

using namespace std;
using namespace core;

GrpcServer::GrpcServer(const string& listeningPointAddress)
{
	m_running = false;
	m_builder.AddListeningPort(listeningPointAddress, grpc::InsecureServerCredentials());
}

GrpcServer::~GrpcServer()
{
}

void GrpcServer::AddService(unique_ptr<grpc::Service> service)
{
	ASSERT(m_running == false);
	m_builder.RegisterService(service.get());
	m_services.emplace_back(service.release());
}

void GrpcServer::Start()
{
	ASSERT(m_running.exchange(true) == false);
	m_server = m_builder.BuildAndStart();
}
