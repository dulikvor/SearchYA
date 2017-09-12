#include "DBClientReply.h"
#include "Core/Assert.h"

using namespace std;

DBClientReply::DBClientReply(const char *rawBuffer, int length, int typeID)
        : m_typeID(typeID){
    static_assert(REDIS_REPLY_STRING == VerifyHelper<GeneralReturnTypesCollection>::GetTypeID<string>(), "Mismatch between redis reply type and current type collection order");
    static_assert(REDIS_REPLY_ARRAY== VerifyHelper<GeneralReturnTypesCollection>::GetTypeID<BulkStringVector>(), "Mismatch between redis reply type and current type collection order");
    static_assert(REDIS_REPLY_INTEGER== VerifyHelper<GeneralReturnTypesCollection>::GetTypeID<int>(), "Mismatch between redis reply type and current type collection order");
    m_rawBuffer.emplace_back(make_pair((char*)malloc(sizeof(char) * length), length));
    memcpy(m_rawBuffer.back().first, rawBuffer, length);
}

DBClientReply::DBClientReply(redisReply **replyNodes, int length, int typeID): m_typeID(typeID) {
    static_assert(REDIS_REPLY_ARRAY == VerifyHelper<GeneralReturnTypesCollection>::GetTypeID<BulkStringVector>(), "Mismatch between redis reply type and current type collection order");
    for(int index = 0; index < length; index++)
    {
        redisReply*& currentReply = replyNodes[index];
        VERIFY(currentReply->type == REDIS_REPLY_STRING, "Non string type reply node is not supported");
        m_rawBuffer.emplace_back(make_pair((char*)malloc(sizeof(char) * currentReply->len), currentReply->len));
        memcpy(m_rawBuffer.back().first, currentReply->str, currentReply->len);
    }
}

DBClientReply::~DBClientReply() {
    for(auto& buffer : m_rawBuffer)
        free(buffer.first);
}
