#include "TopKTask.h"
#include "Communication/DBClient.h"
#include "Communication/GeneralParams.h"
#include "Communication/Serializor.h"
#include "RedisSearchModule/Document.h"
#include "IndexBuilder.h"
#include "Executor.h"

using namespace std;

TopKTask::TopKTask(const std::string &id, float coreCount, const char *data, int length)
    : Task(id, coreCount){
    Serializor serializor(data, length);
    GeneralParams params;
    params.Deserialize(serializor);

    m_word = StringConverter::Convert(params.GetValue("Word"));
    m_k = params.GetValue("Top K");
}

void TopKTask::Run() {
    vector<pair<const char*, int>> arguments;
    Serializor serializor;
    Serializor::Serialize(serializor, m_word);
    string wordBuffer = serializor.ToString();
    arguments.push_back(make_pair(wordBuffer.data(), wordBuffer.size()));
    serializor.Clean();

    Serializor::Serialize(serializor, m_k);
    string kBuffer = serializor.ToString();
    arguments.push_back(make_pair(kBuffer.data(), kBuffer.size()));
    serializor.Clean();


    TRACE_INFO("Requested Word - %s, K - %d", m_word.c_str(), m_k);
    vector<string> documentsBuffer = IndexBuilder::Instance().GetDBClient().CustomCommand(
            "Search.GetTopKDocuments", arguments);

    Serializor::Serialize(serializor, documentsBuffer);
    IndexBuilder::Instance().GetExecutor().SendMessage(serializor.ToString());
    serializor.Clean();

    vector<Document> documents;
    for(const string& documentBuffer : documentsBuffer){
        documents.emplace_back(Document::Deserialize(documentBuffer.data(), documentBuffer.size()));
        TRACE_INFO("Fetched document Name - %s, Path - %s", documents.back().GetName().c_str(),
        documents.back().GetPath().c_str());
    }
}

