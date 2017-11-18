#include "InitJob.h"
#include "Communication/Serializor.h"
#include "ConfigParams.h"

using namespace std;

unique_ptr<pair<const char*, int>, Job::Deleter> InitJob::GenerateTaskData() const
{
    Serializor serializor;
    Serializor::Serialize(serializor, ConfigParams::Instance().GetRedisServerAddress());

    string serializorBuffer = serializor.ToString();
    unique_ptr<char[]> buffer(new char[serializorBuffer.size()]);
    memcpy(buffer.get(), serializorBuffer.c_str(), serializorBuffer.size());
    return unique_ptr<pair<const char*, int>, Job::Deleter>(new pair<const char*, int>(buffer.release(),
      (int)serializorBuffer.size()));
};
