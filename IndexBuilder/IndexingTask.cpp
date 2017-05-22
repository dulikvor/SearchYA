#include "IndexingTask.h"
#include "Communication/DBClient.h"
#include "RedisSearchModule/Document.h"
#include "IndexBuilder.h"

using namespace std;

void IndexingTask::Run(function<void(void)> *const specificOnInit) {
    function<void(void)> onInit = bind(&IndexingTask::OnInit, this, nullptr);
    Task::Run(&onInit);
}

void IndexingTask::OnInit(vector<function<void(void)>> *const specificOnProcessingList) {
    vector<function<void(void)>> onProcessingList = {bind(&IndexingTask::OnProcessing, this, 0, nullptr)};
    Task::OnInit(&onProcessingList);
}

void IndexingTask::OnProcessing(int subTaskID, std::function<void(void)> *const processingFunctio) {
    function<void(void)> processingFunctor = bind(&IndexingTask::IndexDocument, this);
    Task::OnProcessing(subTaskID, &processingFunctor);
}

void IndexingTask::IndexDocument() {
    static int documentNumber = 0;
    Document document(to_string(documentNumber++));
    document.AddWord("Hello");
    document.AddWord("World");
    string serializedDocument = Document::Serialize(document);
    IndexBuilder::Instance().GetDBClient().CustomCommand("Search.AddDocument",
             {make_pair(serializedDocument.c_str(), serializedDocument.length())});

}