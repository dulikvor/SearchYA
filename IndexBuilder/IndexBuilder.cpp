#include "IndexBuilder.h"
#include <functional>
#include <chrono>
#include "Core/Logger.h"
#include "Core/AsyncTask.h"
#include "Communication/GeneralParams.h"
#include "Communication/DBClient.h"
#include "Communication/Serializor.h"
#include "Communication/MessageType.h"
#include "ConfigParams.h"
#include "Command.h"

using namespace core;
using namespace std;

IndexBuilder& IndexBuilder::Instance()
{
	static IndexBuilder instance;
	return instance;
}

IndexBuilder::IndexBuilder()
	:m_state(RunningState::POWERED)
{
}

IndexBuilder::~IndexBuilder()
{
}

void IndexBuilder::InitializeMesos()
{
	m_executor.Init();
}


void IndexBuilder::WaitForCompletion()
{
    unique_lock<mutex> localLock(m_mutex);
    m_conditionVar.wait(localLock, [&]{return m_state == RunningState::TERMINATED;});
}

void IndexBuilder::NewCommand(CommandType commandType, const GeneralParams &params)
{
	Command command(commandType, params);
	AsyncTask asyncTask(bind(&IndexBuilder::HandleCommand, this, cref(command)));
	m_asyncExecutor.SpawnTask(&asyncTask);
	asyncTask.Wait();
}

void IndexBuilder::HandleCommand(const Command& command)
{
    m_stateMachine.HandleState(command.commandType, command.params);
}

void IndexBuilder::Init(const GeneralParams& params)
{
	ConfigParams::Instance().Load(params);
	m_dbClient.Connect(ConfigParams::Instance().GetRedisHostAddress());
}


void IndexBuilder::Processing(const GeneralParams& params)
{
	int coreCount = params.GetValue("Core Count");
	string taskID = StringConverter::Convert(params.GetValue("Task ID"));
	m_processingManager.SubmitNewTask(taskID, coreCount);
}

void IndexBuilder::Terminate()
{
    unique_lock<mutex> localLock(m_mutex);
	Logger::Instance().Flush();
    m_state = RunningState::TERMINATED;
    m_conditionVar.notify_one();
}
