#include "GetTopKTask.h"
#include <vector>
#include "Communication/TextualSearchResultsImpl.h"
#include "Communication/Serializor.h"

using namespace std;

void GetTopKTask::ProcessTask(const std::string &data) {
    TextualSearchResultsImpl& service =  static_cast<TextualSearchResultsImpl&>(*m_service.get());
    Serializor serializor(data.data(), data.length());
    vector<string> documentsNames = Serializor::DeserializeVectorString(serializor);
    service.ProvideResult(documentsNames);
}
