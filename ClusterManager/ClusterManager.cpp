#include "ClusterManager.h"
#include <functional>
#include "Core/Exception.h"
#include "Core/Assert.h"
#include "Core/Exception.h"
#include "Core/Assert.h"
#include "Core/AsyncTask.h"
#include "Core/Logger.h"
#include "Communication/CommandsImpl.h"
#include "Communication/Serializor.h"
#include "Communication/GeneralParams.h"
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

void ClusterManager::Init()
{
	m_scheduler.reset(new Scheduler(ConfigParams::Instance().GetRole()));
	m_scheduler->Initialize();
}

void ClusterManager::InitializeServer(const string& serverListeningPoint)
{
	m_server.reset(new GrpcServer(serverListeningPoint));
	m_server->AddService(unique_ptr<grpc::Service>(new CommandsImpl()));
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


void ClusterManager::HandleMesosMessage(const Scheduler::MessageSource& source, 
		const std::string& data)
{
	Serializor serializor(data.data(), data.size());
	int stubType = Serializor::DeserializeInt(serializor);
	MessageType messageType = (MessageType::Enumeration)Serializor::DeserializeInt(serializor);
	GeneralParams params;
	if(data.size() > MessageType::HeaderSize) //more than message type
	{
		params.Deserialize(serializor);
	}
	params.AddParam("Slave ID", source.SlaveID);
	params.AddParam("Executor ID", source.ExecutorID);
	m_stateMachine.HandleState(CommandType::FromMessageType(messageType), params);
}

void ClusterManager::WaitForCompletion()
{
    unique_lock<mutex> localLock(m_mutex);
    m_conditionVar.wait(localLock, [&]{return m_state == RunningState::TERMINATED;});
}

void ClusterManager::Terminate()
{
    unique_lock<mutex> localLock(m_mutex);
    m_state = RunningState::TERMINATED;
    m_conditionVar.notify_one();
}


void ClusterManager::HandleCommand(const Command& command)
{
    m_stateMachine.HandleState(command.commandType, command.params);
}
