#include "GetTopKJob.h"
#include "Communication/GeneralParams.h"
#include "Communication/Serializor.h"

using namespace std;

GetTopKJob::GetTopKJob(int id, GeneralParams const *const params)
    :Job(id, 0.1){
    m_word = StringConverter::Convert(params->GetValue("Word"));
    m_k = params->GetValue("Top K");
}

unique_ptr<pair<const char*, int>, Job::Deleter> GetTopKJob::GenerateTaskData() const
{
    GeneralParams params;
    params.AddParam("Word", m_word);
    params.AddParam("Top K", m_k);

    Serializor serializor;
    params.Serialize(serializor);

    string serializorBuffer = serializor.GetBuffer();
    unique_ptr<char[]> buffer(new char[serializorBuffer.size()]);
    memcpy(buffer.get(), serializorBuffer.c_str(), serializorBuffer.size());
    return unique_ptr<pair<const char*, int>, Job::Deleter>(new pair<const char*, int>(buffer.release(),
           (int)serializor.GetBuffer().size()));
};
