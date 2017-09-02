#include "InitTask.h"
#include "Communication/Serializor.h"
#include "Communication/GeneralParams.h"
#include "IndexBuilder.h"

using namespace std;
using namespace core;

InitTask::InitTask(const string& id,  float coreCount,
                   const char* data, int length):Task(id, coreCount) {
    Serializor serializor(data, length);
    m_dbServerAddress = Serializor::DeserializeString(serializor);
}
void InitTask::Run() {
    TRACE_INFO("Connecting to - %s", m_dbServerAddress.c_str());
    IndexBuilder::Instance().GetDBClient().Connect(m_dbServerAddress);
}

