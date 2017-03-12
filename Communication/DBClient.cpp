#include "DBClient.h"
#include "hiredis-0.13.3/hiredis.h"
#include "Core/Exception.h"
#include "Core/Logger.h"
#include "Core/Assert.h"

using namespace std;
using namespace core;

DBClient::~DBClient()
{
	redisFree(m_redisContext);
}

void DBClient::Connect(const string& hostAddress)
{
	m_redisContext = redisConnect(hostAddress.c_str(), 6379);
	if(m_redisContext == nullptr)
	{
		throw Exception(SOURCE, "Couldn't allocate redis server context");
	}
	else if(m_redisContext->err)
	{
		throw Exception(SOURCE, "Couldn't connect to redis server - %s:6379, reason - %s", 
				hostAddress.c_str(), m_redisContext->errstr);
	}
	TRACE_INFO("Connected to redis server - %s:6379", hostAddress.c_str());
}

bool DBClient::SetHashFields(const string& key, 
		const vector<pair<string, const vector<char>>>& fieldsValues)
{
	string command = string("HMSET") + " " + key;
	for(const pair<string, const vector<char>>& fieldValuePair : fieldsValues)
	{
		command += string(" ") + fieldValuePair.first + " " + 
			string(fieldValuePair.second.begin(), fieldValuePair.second.end());
	}
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, command.c_str());
	if(reply == nullptr)
	{
		throw Exception(SOURCE, "Invalid reply was received.");
	}
	else if(m_redisContext->err)
	{
		throw Exception(SOURCE, "Redis error - %s", m_redisContext->errstr);
	}
	VERIFY(reply->type == REDIS_REPLY_INTEGER, "Received redis reply dosn't match expected return type");
	return (bool)reply->integer;
}

bool DBClient::DelHashFields(const string& key, const vector<string>& fields)
{
	string command = string("HDEL") + " " + key;
	for(const string& field : fields)
	{
		command += string(" ") + field;
	}
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, command.c_str());
	if(reply == nullptr)
	{
		throw Exception(SOURCE, "Invalid reply was received.");
	}
	else if(m_redisContext->err)
	{
		throw Exception(SOURCE, "Redis error - %s", m_redisContext->errstr);
	}
	VERIFY(reply->type == REDIS_REPLY_INTEGER, "Received redis reply dosn't match expected return type");
	return (bool)reply->integer;
}
