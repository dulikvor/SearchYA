#pragma once

#include "Param.h"

struct redisReply;
class RedisClient;

class RedisReplyParams
{
public:
	RedisReplyParams(){}
    ~RedisReplyParams(){}

private:
	friend RedisClient;
    void Parse(const redisReply* redisReply);
private:
	RedisType m_type;
	std::string m_stringValue;
	long long m_numericValue;
	std::vector<RedisReplyParam> m_replyArray;
};

