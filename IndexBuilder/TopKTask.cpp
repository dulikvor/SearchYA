#include "TopKTask.h"
#include "Communication/DBClient.h"
#include "Communication/GeneralParams.h"
#include "Communication/Serializor.h"
#include "RedisSearchModule/Document.h"
#include "IndexBuilder.h"

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
    string buffer = serializor.GetBuffer();
    arguments.push_back(make_pair(buffer.data(), buffer.size()));
    serializor.Clean();

    Serializor::Serialize(serializor, m_k);
    buffer = serializor.GetBuffer();
    arguments.push_back(make_pair(buffer.data(), buffer.size()));
    serializor.Clean();

    vector<string> documentsBuffer = IndexBuilder::Instance().GetDBClient().CustomCommand(
            "Search.GetTopKDocuments", arguments);
    vector<Document> documents;
    for(const string& documentBuffer : documentsBuffer){
        documents.emplace_back(Document::Deserialize(documentBuffer.data(), documentBuffer.size()));
    }
}

