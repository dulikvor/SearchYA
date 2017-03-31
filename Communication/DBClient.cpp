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
	unique_lock<mutex> lock(m_mutex);
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


bool DBClient::SetString(const std::string& key, const char* value, int size)
{
	unique_lock<mutex> lock(m_mutex);
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, "SET %s %b", key.c_str(),
			value, size);
	if(reply == nullptr)
	{
		throw Exception(SOURCE, "Invalid reply was received.");
	}
	else if(m_redisContext->err)
	{
		throw Exception(SOURCE, "Redis error - %s", m_redisContext->errstr);
	}
	VERIFY(reply->type == REDIS_REPLY_STATUS, 
			"Received redis reply dosn't match expected return type");

	TRACE_INFO("SET was done %s", reply->str);
	return strcmp(reply->str, "OK") == 0;
}

bool DBClient::SetHashFields(const string& key, 
		const vector<tuple<const string&, char*, int>>& fieldsValues)
{
	unique_lock<mutex> lock(m_mutex);
	string command = string("HMSET") + " " + key;
	for(const tuple<const string&, char*, int>& fieldValuePair : fieldsValues) //build ansi format
	{
		command += string(" ") + get<0>(fieldValuePair) + " %b";
	}
	const char** argv = (const char**)malloc(sizeof(char*)*(fieldsValues.size() + 1));
	size_t* argvlen = (size_t*)malloc(sizeof(size_t)*fieldsValues.size() + 1);
	//Add ansi format first
	argv[0] = command.c_str();
	argvlen[0] = command.size();
	int index = 1;
	for(const tuple<const string&, char*, int>& fieldValuePair : fieldsValues)
	{
		argv[index] = get<1>(fieldValuePair);
		argvlen[index] = (size_t)get<2>(fieldValuePair);
		index++;
	}
	redisReply* reply = (redisReply*)redisCommandArgv(m_redisContext, 
			(int)(fieldsValues.size() + 1), argv, argvlen);

	free(argv);
	free(argvlen);
	if(reply == nullptr)
	{
		throw Exception(SOURCE, "Invalid reply was received.");
	}
	else if(m_redisContext->err)
	{
		throw Exception(SOURCE, "Redis error - %s", m_redisContext->errstr);
	}
	VERIFY(reply->type == REDIS_REPLY_INTEGER, 
			"Received redis reply dosn't match expected return type");
	return (bool)reply->integer;
}

bool DBClient::DelHashFields(const string& key, const vector<string>& fields)
{
	unique_lock<mutex> lock(m_mutex);
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
