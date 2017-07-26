#include "IndexBuilder.h"
#include "Core/Logger.h"

using namespace core;
using namespace std;

IndexBuilder& IndexBuilder::Instance()
{
	static IndexBuilder instance;
	return instance;
}

void IndexBuilder::InitializeMesos()
{
	m_executor.Init();
}


void IndexBuilder::WaitForCompletion()
{
    unique_lock<mutex> localLock(m_mutex);
    m_conditionVar.wait(localLock, [this]{return (bool)m_terminate;});
}

void IndexBuilder::Terminate()
{
    unique_lock<mutex> localLock(m_mutex);
	Logger::Instance().Flush();
    m_terminate = true;
    m_conditionVar.notify_one();
}
