#include "ClusterManager.h"
#include <functional>
#include "Core/Exception.h"
#include "Core/Assert.h"
#include "Core/Exception.h"
#include "Core/Assert.h"
#include "Core/AsyncTask.h"
#include "Core/Logger.h"
#include "Communication/Serializor.h"
#include "Communication/GeneralParams.h"
#include "Communication/TextualSearchServiceImpl.h"
#include "Communication/TextualSearchResultsImpl.h"
#include "RedisSearchModule/Document.h"
#include "ConfigParams.h"
#include "Command.h"

using namespace std;
using namespace core;

ClusterManager& ClusterManager::Instace()
{
    static ClusterManager instance;
    return instance;
}

ClusterManager::ClusterManager()
    :m_state(RunningState::RUNNING)
{
}

ClusterManager::~ClusterManager()
{
}

void ClusterManager::Init(const GeneralParams& params)
{
    NewCommand(CommandType::Init, params);
}

void ClusterManager::IndexDocument(const GeneralParams& params)
{
    NewCommand(CommandType::Index, params);
}

void ClusterManager::GetTopKDocuments(const string& word, int k, void* callbackTag)
{
	GeneralParams params;
	params.AddParam("Word", word);
	params.AddParam("Top K", k);
	params.AddParam("CallBack Tag", callbackTag);
	NewCommand(CommandType::GetTopK, params);
}

void ClusterManager::Terminate()
{
    NewCommand(CommandType::Terminate, GeneralParams());
}

void ClusterManager::HandleInit()
{
	m_scheduler.reset(new Scheduler(ConfigParams::Instance().GetRole()));
	m_scheduler->Initialize();
}

void ClusterManager::InitializeServer(const string& serverListeningPoint)
{
	m_server.reset(new GrpcServer(serverListeningPoint));
	m_server->AddService(shared_ptr<Service>(new TextualSearchServiceImpl(*this)));
	m_server->AddAsyncService(shared_ptr<AsyncService>(new TextualSearchResultsImpl(m_server->GetCompletionQueue(), *this)));
	m_server->Start();
	TRACE_INFO("GRPC Server is up at - %s", serverListeningPoint.c_str());
}

void ClusterManager::NewCommand(CommandType commandType, const GeneralParams &params)
{
	Command command(commandType, params);
	AsyncTask asyncTask(bind(&ClusterManager::HandleCommand, this, cref(command)));
	m_asyncExecutor.SpawnTask(&asyncTask);
	asyncTask.Wait();
}

void ClusterManager::WaitForCompletion()
{
    unique_lock<mutex> localLock(m_mutex);
    m_conditionVar.wait(localLock, [&]{return m_state == RunningState::TERMINATED;});
}

void ClusterManager::HandleTerminate()
{
    unique_lock<mutex> localLock(m_mutex);
    m_state = RunningState::TERMINATED;
    m_conditionVar.notify_one();
}


void ClusterManager::HandleCommand(const Command& command)
{
    m_stateMachine.HandleState(command.commandType, command.params);
}
