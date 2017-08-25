#include "DBClient.h"
#include <limits>
#include <functional>
#include "hiredis/hiredis.h"
#include "Core/Exception.h"
#include "Core/Logger.h"
#include "Core/Deleter.h"
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
	auto deleterFunc = [](redisReply *reply)->void{freeReplyObject(reply);};
	Deleter<redisReply> deleter(reply, deleterFunc);
	if(reply == nullptr)
		throw Exception(SOURCE, "Invalid reply was received.");
	else if(reply->type == REDIS_REPLY_ERROR)
		throw Exception(SOURCE, "Redis error - %s", reply->str);

	TRACE_INFO("SET was done %s", reply->str);
	VERIFY(reply->type == REDIS_REPLY_STRING,
		   "Received redis reply dosn't match expected return type");
	bool success = strcmp(reply->str, "OK") == 0;
	return success;
}

DBClientReply DBClient::CustomCommand(const std::string& command, const std::vector<std::pair<const char*, int>>& arguments)
{
	unique_lock<mutex> lock(m_mutex);
	const char** argv = (const char**)malloc(sizeof(char*)*(arguments.size() + 1));
	size_t* argvlen = (size_t*)malloc(sizeof(size_t)*arguments.size() + 1);
	//Add ansi format first
	argv[0] = command.c_str();
	argvlen[0] = command.size();
	int index = 1;
	for(auto& argument : arguments)
	{
		argv[index] = argument.first;
		argvlen[index] = (size_t)argument.second;
		index++;
	}
	redisReply* reply = (redisReply*)redisCommandArgv(m_redisContext, 
			(int)(arguments.size() + 1), argv, argvlen);
	constexpr auto deleterFunc = [](redisReply *reply){freeReplyObject(reply);};
	Deleter<redisReply> deleter(reply, deleterFunc);

	VERIFY(reply->type == REDIS_REPLY_INTEGER && reply->integer <= numeric_limits<int>::max(), "reply long long value is too large for an int");
	if(reply->type == REDIS_REPLY_INTEGER || reply->type == REDIS_REPLY_STRING)
		return DBClientReply(reply->type == REDIS_REPLY_INTEGER ? reinterpret_cast<const char*>(&reply->integer) : reply->str, reply->len, reply->type);
	else
		return DBClientReply(reply->element, reply->elements);


}

bool DBClient::SetHashFields(const string& key, 
		const vector<tuple<const string&, char*, int>>& fieldsValues)
{
	unique_lock<mutex> lock(m_mutex);
	string clientCommand = string("HMSET") + " " + key;
	for(const tuple<const string&, char*, int>& fieldValuePair : fieldsValues) //build ansi format
		clientCommand += string(" ") + get<0>(fieldValuePair) + " %b";
	
	const char** argv = (const char**)malloc(sizeof(char*)*(fieldsValues.size() + 1));
	size_t* argvlen = (size_t*)malloc(sizeof(size_t)*fieldsValues.size() + 1);
	//Add ansi format first
	argv[0] = clientCommand.c_str();
	argvlen[0] = clientCommand.size();
	int index = 1;
	for(const tuple<const string&, char*, int>& fieldValuePair : fieldsValues)
	{
		argv[index] = get<1>(fieldValuePair);
		argvlen[index] = (size_t)get<2>(fieldValuePair);
		index++;
	}
	redisReply* reply = (redisReply*)redisCommandArgv(m_redisContext, 
			(int)(fieldsValues.size() + 1), argv, argvlen);
	constexpr auto deleterFunc = [](redisReply *reply){freeReplyObject(reply);};
	Deleter<redisReply> deleter(reply, deleterFunc);
	free(argv);
	free(argvlen);

	if(reply == nullptr)
		throw Exception(SOURCE, "Invalid reply was received.");
	else if(m_redisContext->err)
		throw Exception(SOURCE, "Redis error - %s", m_redisContext->errstr);

	VERIFY(reply->type == REDIS_REPLY_INTEGER,
			"Received redis reply dosn't match expected return type");
	return (bool)reply->integer;
}

bool DBClient::DelHashFields(const string& key, const vector<string>& fields)
{
	unique_lock<mutex> lock(m_mutex);
	string clientCommand = string("HDEL") + " " + key;
	for(const string& field : fields)
	{
		clientCommand += string(" ") + field;
	}
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, clientCommand.c_str());
	function<void(redisReply*)> deleterFunc = [](redisReply *reply){freeReplyObject(reply);};
	Deleter<redisReply> deleter(reply, deleterFunc);

	if(reply == nullptr)
		throw Exception(SOURCE, "Invalid reply was received.");
	else if(m_redisContext->err)
		throw Exception(SOURCE, "Redis error - %s", m_redisContext->errstr);

	VERIFY(reply->type == REDIS_REPLY_INTEGER, "Received redis reply dosn't match expected return type");
	return (bool)reply->integer;
}
