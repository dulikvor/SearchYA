#include "ClusterManager.h"
#include <functional>
#include "Core/Exception.h"
#include "Core/Assert.h"
#include "Core/AsyncTask.h"
#include "ConfigParams.h"
#include "Command.h"

using namespace std;
using namespace Core;

ClusterManager& ClusterManager::Instace()
{
    static ClusterManager instance;
    return instance;
}

ClusterManager::ClusterManager()
    :m_state(State::RUNNING)
{
}

ClusterManager::~ClusterManager()
{
}

void ClusterManager::Init()
{
	//m_scheduler.Initialize();
}

void ClusterManager::NewCommand(CommandType commandType, const Params &params)
{
    Command command(commandType, params);
    AsyncTask* asyncTask = new AsyncTask(bind(&ClusterManager::HandleCommand, this, cref(command)));
    m_asyncExecutor.SpawnTask(asyncTask);
    asyncTask->Wait();
    delete asyncTask;
}

void ClusterManager::WaitForCompletion()
{
    unique_lock<mutex> localLock(m_mutex);
    m_conditionVar.wait(localLock, [&]{return m_state == State::TERMINATED;});
}

void ClusterManager::Terminate()
{
    unique_lock<mutex> localLock(m_mutex);
    m_state = State::TERMINATED;
    m_conditionVar.notify_one();
}


void ClusterManager::HandleCommand(const Command& command)
{
    m_stateMachine.HandleState(command.commandType, command.params);
}
