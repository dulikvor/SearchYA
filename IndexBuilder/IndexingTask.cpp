#include "IndexingTask.h"
#include "Communication/DBClient.h"
#include "RedisSearchModule/Document.h"
#include "IndexBuilder.h"

using namespace std;

void IndexingTask::Run() {
    static int documentNumber = 0;
    Document document(to_string(documentNumber++), "Path");
    document.AddWord("Hello");
    document.AddWord("World");
    string serializedDocument = Document::Serialize(document);
    IndexBuilder::Instance().GetDBClient().CustomCommand("Search.AddDocument",
         {make_pair(serializedDocument.c_str(), serializedDocument.length())});

}
