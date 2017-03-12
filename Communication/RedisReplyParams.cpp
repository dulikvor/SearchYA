#include "RedisReplyParams.h"
#include "hiredis-0.13.3/hiredis.h"

using namespace std;

#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6
void RedisParam::Parse(const redisReply* redisReply)
{
	switch(redisReply->type)
	{
		case REDIS_REPLY_STRING:
		{
			m_stringValue = string(redisReply->str, redisReply->len);
			break;
		}
		case 
	}
}
