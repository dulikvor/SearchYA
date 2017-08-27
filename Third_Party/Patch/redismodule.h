#ifndef REDISMODULE_H
#define REDISMODULE_H

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

/* ---------------- Defines common between core and modules --------------- */

/* Error status return values. */
#define REDISMODULE_OK 0
#define REDISMODULE_ERR 1

/* API versions. */
#define REDISMODULE_APIVER_1 1

/* API flags and constants */
#define REDISMODULE_READ (1<<0)
#define REDISMODULE_WRITE (1<<1)

#define REDISMODULE_LIST_HEAD 0
#define REDISMODULE_LIST_TAIL 1

/* Key types. */
#define REDISMODULE_KEYTYPE_EMPTY 0
#define REDISMODULE_KEYTYPE_STRING 1
#define REDISMODULE_KEYTYPE_LIST 2
#define REDISMODULE_KEYTYPE_HASH 3
#define REDISMODULE_KEYTYPE_SET 4
#define REDISMODULE_KEYTYPE_ZSET 5
#define REDISMODULE_KEYTYPE_MODULE 6

/* Reply types. */
#define REDISMODULE_REPLY_UNKNOWN -1
#define REDISMODULE_REPLY_STRING 0
#define REDISMODULE_REPLY_ERROR 1
#define REDISMODULE_REPLY_INTEGER 2
#define REDISMODULE_REPLY_ARRAY 3
#define REDISMODULE_REPLY_NULL 4

/* Postponed array length. */
#define REDISMODULE_POSTPONED_ARRAY_LEN -1

/* Expire */
#define REDISMODULE_NO_EXPIRE -1

/* Sorted set API flags. */
#define REDISMODULE_ZADD_XX      (1<<0)
#define REDISMODULE_ZADD_NX      (1<<1)
#define REDISMODULE_ZADD_ADDED   (1<<2)
#define REDISMODULE_ZADD_UPDATED (1<<3)
#define REDISMODULE_ZADD_NOP     (1<<4)

/* Hash API flags. */
#define REDISMODULE_HASH_NONE       0
#define REDISMODULE_HASH_NX         (1<<0)
#define REDISMODULE_HASH_XX         (1<<1)
#define REDISMODULE_HASH_CFIELDS    (1<<2)
#define REDISMODULE_HASH_EXISTS     (1<<3)

/* A special pointer that we can use between the core and the module to signal
 * field deletion, and that is impossible to be a valid pointer. */
#define REDISMODULE_HASH_DELETE ((RedisModuleString*)(long)1)

/* Error messages. */
#define REDISMODULE_ERRORMSG_WRONGTYPE "WRONGTYPE Operation against a key holding the wrong kind of value"

#define REDISMODULE_POSITIVE_INFINITE (1.0/0.0)
#define REDISMODULE_NEGATIVE_INFINITE (-1.0/0.0)

#define REDISMODULE_NOT_USED(V) ((void) V)

/* ------------------------- End of common defines ------------------------ */

#ifndef REDISMODULE_CORE

typedef long long mstime_t;

/* Incomplete structures for compiler checks but opaque access. */
typedef struct RedisModuleCtx RedisModuleCtx;
typedef struct RedisModuleKey RedisModuleKey;
typedef struct RedisModuleString RedisModuleString;
typedef struct RedisModuleCallReply RedisModuleCallReply;
typedef struct RedisModuleIO RedisModuleIO;
typedef struct RedisModuleType RedisModuleType;
typedef struct RedisModuleDigest RedisModuleDigest;
typedef struct RedisModuleBlockedClient RedisModuleBlockedClient;

typedef int (*RedisModuleCmdFunc) (RedisModuleCtx *ctx, RedisModuleString **argv, int argc);

typedef void *(*RedisModuleTypeLoadFunc)(RedisModuleIO *rdb, int encver);
typedef void (*RedisModuleTypeSaveFunc)(RedisModuleIO *rdb, void *value);
typedef void (*RedisModuleTypeRewriteFunc)(RedisModuleIO *aof, RedisModuleString *key, void *value);
typedef size_t (*RedisModuleTypeMemUsageFunc)(const void *value);
typedef void (*RedisModuleTypeDigestFunc)(RedisModuleDigest *digest, void *value);
typedef void (*RedisModuleTypeFreeFunc)(void *value);

#define REDISMODULE_TYPE_METHOD_VERSION 1
typedef struct RedisModuleTypeMethods {
    uint64_t version;
    RedisModuleTypeLoadFunc rdb_load;
    RedisModuleTypeSaveFunc rdb_save;
    RedisModuleTypeRewriteFunc aof_rewrite;
    RedisModuleTypeMemUsageFunc mem_usage;
    RedisModuleTypeDigestFunc digest;
    RedisModuleTypeFreeFunc free;
} RedisModuleTypeMethods;

#define REDISMODULE_GET_API(name) \
    RedisModule_GetApi("RedisModule_" #name, ((void **)&RedisModule_ ## name))

#define REDISMODULE_API_FUNC(x) (*x)

#ifdef REDIS_EXTERN
    #define EXTERN extern
#else
    #define EXTERN
#endif
EXTERN   void *REDISMODULE_API_FUNC(RedisModule_Alloc)(size_t bytes);
EXTERN   void *REDISMODULE_API_FUNC(RedisModule_Realloc)(void *ptr, size_t bytes);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_Free)(void *ptr);
EXTERN   void *REDISMODULE_API_FUNC(RedisModule_Calloc)(size_t nmemb, size_t size);
EXTERN   char *REDISMODULE_API_FUNC(RedisModule_Strdup)(const char *str);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_GetApi)(const char *, void *);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_CreateCommand)(RedisModuleCtx *ctx, const char *name, RedisModuleCmdFunc cmdfunc,
            const char *strflags, int firstkey, int lastkey, int keystep);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_SetModuleAttribs)(RedisModuleCtx *ctx, const char *name, int ver, int apiver);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_WrongArity)(RedisModuleCtx *ctx);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithLongLong)(RedisModuleCtx *ctx, long long ll);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_GetSelectedDb)(RedisModuleCtx *ctx);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_SelectDb)(RedisModuleCtx *ctx, int newid);
EXTERN   void *REDISMODULE_API_FUNC(RedisModule_OpenKey)(RedisModuleCtx *ctx, RedisModuleString *keyname, int mode);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_CloseKey)(RedisModuleKey *kp);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_KeyType)(RedisModuleKey *kp);
EXTERN   size_t REDISMODULE_API_FUNC(RedisModule_ValueLength)(RedisModuleKey *kp);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ListPush)(RedisModuleKey *kp, int where, RedisModuleString *ele);
EXTERN   RedisModuleString *REDISMODULE_API_FUNC(RedisModule_ListPop)(RedisModuleKey *key, int where);
EXTERN   RedisModuleCallReply *REDISMODULE_API_FUNC(RedisModule_Call)(RedisModuleCtx *ctx, const char *cmdname, const char *fmt, ...);
EXTERN   const char *REDISMODULE_API_FUNC(RedisModule_CallReplyProto)(RedisModuleCallReply *reply, size_t *len);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_FreeCallReply)(RedisModuleCallReply *reply);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_CallReplyType)(RedisModuleCallReply *reply);
EXTERN   long long REDISMODULE_API_FUNC(RedisModule_CallReplyInteger)(RedisModuleCallReply *reply);
EXTERN   size_t REDISMODULE_API_FUNC(RedisModule_CallReplyLength)(RedisModuleCallReply *reply);
EXTERN   RedisModuleCallReply *REDISMODULE_API_FUNC(RedisModule_CallReplyArrayElement)(RedisModuleCallReply *reply, size_t idx);
EXTERN   RedisModuleString *REDISMODULE_API_FUNC(RedisModule_CreateString)(RedisModuleCtx *ctx, const char *ptr, size_t len);
EXTERN   RedisModuleString *REDISMODULE_API_FUNC(RedisModule_CreateStringFromLongLong)(RedisModuleCtx *ctx, long long ll);
EXTERN   RedisModuleString *REDISMODULE_API_FUNC(RedisModule_CreateStringFromString)(RedisModuleCtx *ctx, const RedisModuleString *str);
EXTERN   RedisModuleString *REDISMODULE_API_FUNC(RedisModule_CreateStringPrintf)(RedisModuleCtx *ctx, const char *fmt, ...);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_FreeString)(RedisModuleCtx *ctx, RedisModuleString *str);
EXTERN   const char *REDISMODULE_API_FUNC(RedisModule_StringPtrLen)(const RedisModuleString *str, size_t *len);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithError)(RedisModuleCtx *ctx, const char *err);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithSimpleString)(RedisModuleCtx *ctx, const char *msg);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithArray)(RedisModuleCtx *ctx, long len);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_ReplySetArrayLength)(RedisModuleCtx *ctx, long len);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithStringBuffer)(RedisModuleCtx *ctx, const char *buf, size_t len);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithString)(RedisModuleCtx *ctx, RedisModuleString *str);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithNull)(RedisModuleCtx *ctx);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithDouble)(RedisModuleCtx *ctx, double d);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplyWithCallReply)(RedisModuleCtx *ctx, RedisModuleCallReply *reply);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_StringToLongLong)(const RedisModuleString *str, long long *ll);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_StringToDouble)(const RedisModuleString *str, double *d);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_AutoMemory)(RedisModuleCtx *ctx);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_Replicate)(RedisModuleCtx *ctx, const char *cmdname, const char *fmt, ...);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ReplicateVerbatim)(RedisModuleCtx *ctx);
EXTERN   const char *REDISMODULE_API_FUNC(RedisModule_CallReplyStringPtr)(RedisModuleCallReply *reply, size_t *len);
EXTERN   RedisModuleString *REDISMODULE_API_FUNC(RedisModule_CreateStringFromCallReply)(RedisModuleCallReply *reply);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_DeleteKey)(RedisModuleKey *key);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_StringSet)(RedisModuleKey *key, RedisModuleString *str);
EXTERN   char *REDISMODULE_API_FUNC(RedisModule_StringDMA)(RedisModuleKey *key, size_t *len, int mode);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_StringTruncate)(RedisModuleKey *key, size_t newlen);
EXTERN   mstime_t REDISMODULE_API_FUNC(RedisModule_GetExpire)(RedisModuleKey *key);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_SetExpire)(RedisModuleKey *key, mstime_t expire);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetAdd)(RedisModuleKey *key, double score, RedisModuleString *ele, int *flagsptr);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetIncrby)(RedisModuleKey *key, double score, RedisModuleString *ele, int *flagsptr,
                double *newscore);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetScore)(RedisModuleKey *key, RedisModuleString *ele, double *score);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetRem)(RedisModuleKey *key, RedisModuleString *ele, int *deleted);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_ZsetRangeStop)(RedisModuleKey *key);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetFirstInScoreRange)(RedisModuleKey *key, double min, double max, int minex,
                int maxex);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetLastInScoreRange)(RedisModuleKey *key, double min, double max, int minex,
int maxex);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetFirstInLexRange)(RedisModuleKey *key, RedisModuleString *min,
RedisModuleString *max);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetLastInLexRange)(RedisModuleKey *key, RedisModuleString *min,
RedisModuleString *max);
EXTERN   RedisModuleString *REDISMODULE_API_FUNC(RedisModule_ZsetRangeCurrentElement)(RedisModuleKey *key, double *score);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetRangeNext)(RedisModuleKey *key);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetRangePrev)(RedisModuleKey *key);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ZsetRangeEndReached)(RedisModuleKey *key);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_HashSet)(RedisModuleKey *key, int flags, ...);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_HashGet)(RedisModuleKey *key, int flags, ...);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_IsKeysPositionRequest)(RedisModuleCtx *ctx);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_KeyAtPos)(RedisModuleCtx *ctx, int pos);
EXTERN   unsigned long long REDISMODULE_API_FUNC(RedisModule_GetClientId)(RedisModuleCtx *ctx);
EXTERN   void *REDISMODULE_API_FUNC(RedisModule_PoolAlloc)(RedisModuleCtx *ctx, size_t bytes);
EXTERN   RedisModuleType *REDISMODULE_API_FUNC(RedisModule_CreateDataType)(RedisModuleCtx *ctx, const char *name, int encver,
RedisModuleTypeMethods *typemethods);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_ModuleTypeSetValue)(RedisModuleKey *key, RedisModuleType *mt, void *value);
EXTERN   RedisModuleType *REDISMODULE_API_FUNC(RedisModule_ModuleTypeGetType)(RedisModuleKey *key);
EXTERN   void *REDISMODULE_API_FUNC(RedisModule_ModuleTypeGetValue)(RedisModuleKey *key);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_SaveUnsigned)(RedisModuleIO *io, uint64_t value);
EXTERN   uint64_t REDISMODULE_API_FUNC(RedisModule_LoadUnsigned)(RedisModuleIO *io);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_SaveSigned)(RedisModuleIO *io, int64_t value);
EXTERN   int64_t REDISMODULE_API_FUNC(RedisModule_LoadSigned)(RedisModuleIO *io);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_EmitAOF)(RedisModuleIO *io, const char *cmdname, const char *fmt, ...);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_SaveString)(RedisModuleIO *io, RedisModuleString *s);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_SaveStringBuffer)(RedisModuleIO *io, const char *str, size_t len);
EXTERN   RedisModuleString *REDISMODULE_API_FUNC(RedisModule_LoadString)(RedisModuleIO *io);
EXTERN   char *REDISMODULE_API_FUNC(RedisModule_LoadStringBuffer)(RedisModuleIO *io, size_t *lenptr);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_SaveDouble)(RedisModuleIO *io, double value);
EXTERN   double REDISMODULE_API_FUNC(RedisModule_LoadDouble)(RedisModuleIO *io);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_SaveFloat)(RedisModuleIO *io, float value);
EXTERN   float REDISMODULE_API_FUNC(RedisModule_LoadFloat)(RedisModuleIO *io);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_Log)(RedisModuleCtx *ctx, const char *level, const char *fmt, ...);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_LogIOError)(RedisModuleIO *io, const char *levelstr, const char *fmt, ...);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_StringAppendBuffer)(RedisModuleCtx *ctx, RedisModuleString *str, const char *buf,
size_t len);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_RetainString)(RedisModuleCtx *ctx, RedisModuleString *str);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_StringCompare)(RedisModuleString *a, RedisModuleString *b);
EXTERN   RedisModuleCtx *REDISMODULE_API_FUNC(RedisModule_GetContextFromIO)(RedisModuleIO *io);

EXTERN   RedisModuleBlockedClient *REDISMODULE_API_FUNC(RedisModule_BlockClient)(RedisModuleCtx *ctx, RedisModuleCmdFunc reply_callback,
RedisModuleCmdFunc timeout_callback, void (*free_privdata)(void *),
long long timeout_ms);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_UnblockClient)(RedisModuleBlockedClient *bc, void *privdata);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_IsBlockedReplyRequest)(RedisModuleCtx *ctx);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_IsBlockedTimeoutRequest)(RedisModuleCtx *ctx);
EXTERN   void *REDISMODULE_API_FUNC(RedisModule_GetBlockedClientPrivateData)(RedisModuleCtx *ctx);
EXTERN   int REDISMODULE_API_FUNC(RedisModule_AbortBlock)(RedisModuleBlockedClient *bc);
EXTERN   long long REDISMODULE_API_FUNC(RedisModule_Milliseconds)(void);
EXTERN   RedisModuleCtx *REDISMODULE_API_FUNC(RedisModule_GetThreadSafeContext)(RedisModuleBlockedClient *bc);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_FreeThreadSafeContext)(RedisModuleCtx *ctx);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_ThreadSafeContextLock)(RedisModuleCtx *ctx);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_ThreadSafeContextUnlock)(RedisModuleCtx *ctx);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_DigestAddStringBuffer)(RedisModuleDigest *md, unsigned char *ele, size_t len);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_DigestAddLongLong)(RedisModuleDigest *md, long long ele);
EXTERN   void REDISMODULE_API_FUNC(RedisModule_DigestEndSequence)(RedisModuleDigest *md);
/* This is included inline inside each Redis module. */
static int RedisModule_Init(RedisModuleCtx *ctx, const char *name, int ver, int apiver) __attribute__((unused));
static int RedisModule_Init(RedisModuleCtx *ctx, const char *name, int ver, int apiver) {
    void *getapifuncptr = ((void**)ctx)[0];
    RedisModule_GetApi = (int (*)(const char *, void *)) (unsigned long)getapifuncptr;
    REDISMODULE_GET_API(Alloc);
    REDISMODULE_GET_API(Calloc);
    REDISMODULE_GET_API(Free);
    REDISMODULE_GET_API(Realloc);
    REDISMODULE_GET_API(Strdup);
    REDISMODULE_GET_API(CreateCommand);
    REDISMODULE_GET_API(SetModuleAttribs);
    REDISMODULE_GET_API(WrongArity);
    REDISMODULE_GET_API(ReplyWithLongLong);
    REDISMODULE_GET_API(ReplyWithError);
    REDISMODULE_GET_API(ReplyWithSimpleString);
    REDISMODULE_GET_API(ReplyWithArray);
    REDISMODULE_GET_API(ReplySetArrayLength);
    REDISMODULE_GET_API(ReplyWithStringBuffer);
    REDISMODULE_GET_API(ReplyWithString);
    REDISMODULE_GET_API(ReplyWithNull);
    REDISMODULE_GET_API(ReplyWithCallReply);
    REDISMODULE_GET_API(ReplyWithDouble);
    REDISMODULE_GET_API(ReplySetArrayLength);
    REDISMODULE_GET_API(GetSelectedDb);
    REDISMODULE_GET_API(SelectDb);
    REDISMODULE_GET_API(OpenKey);
    REDISMODULE_GET_API(CloseKey);
    REDISMODULE_GET_API(KeyType);
    REDISMODULE_GET_API(ValueLength);
    REDISMODULE_GET_API(ListPush);
    REDISMODULE_GET_API(ListPop);
    REDISMODULE_GET_API(StringToLongLong);
    REDISMODULE_GET_API(StringToDouble);
    REDISMODULE_GET_API(Call);
    REDISMODULE_GET_API(CallReplyProto);
    REDISMODULE_GET_API(FreeCallReply);
    REDISMODULE_GET_API(CallReplyInteger);
    REDISMODULE_GET_API(CallReplyType);
    REDISMODULE_GET_API(CallReplyLength);
    REDISMODULE_GET_API(CallReplyArrayElement);
    REDISMODULE_GET_API(CallReplyStringPtr);
    REDISMODULE_GET_API(CreateStringFromCallReply);
    REDISMODULE_GET_API(CreateString);
    REDISMODULE_GET_API(CreateStringFromLongLong);
    REDISMODULE_GET_API(CreateStringFromString);
    REDISMODULE_GET_API(CreateStringPrintf);
    REDISMODULE_GET_API(FreeString);
    REDISMODULE_GET_API(StringPtrLen);
    REDISMODULE_GET_API(AutoMemory);
    REDISMODULE_GET_API(Replicate);
    REDISMODULE_GET_API(ReplicateVerbatim);
    REDISMODULE_GET_API(DeleteKey);
    REDISMODULE_GET_API(StringSet);
    REDISMODULE_GET_API(StringDMA);
    REDISMODULE_GET_API(StringTruncate);
    REDISMODULE_GET_API(GetExpire);
    REDISMODULE_GET_API(SetExpire);
    REDISMODULE_GET_API(ZsetAdd);
    REDISMODULE_GET_API(ZsetIncrby);
    REDISMODULE_GET_API(ZsetScore);
    REDISMODULE_GET_API(ZsetRem);
    REDISMODULE_GET_API(ZsetRangeStop);
    REDISMODULE_GET_API(ZsetFirstInScoreRange);
    REDISMODULE_GET_API(ZsetLastInScoreRange);
    REDISMODULE_GET_API(ZsetFirstInLexRange);
    REDISMODULE_GET_API(ZsetLastInLexRange);
    REDISMODULE_GET_API(ZsetRangeCurrentElement);
    REDISMODULE_GET_API(ZsetRangeNext);
    REDISMODULE_GET_API(ZsetRangePrev);
    REDISMODULE_GET_API(ZsetRangeEndReached);
    REDISMODULE_GET_API(HashSet);
    REDISMODULE_GET_API(HashGet);
    REDISMODULE_GET_API(IsKeysPositionRequest);
    REDISMODULE_GET_API(KeyAtPos);
    REDISMODULE_GET_API(GetClientId);
    REDISMODULE_GET_API(PoolAlloc);
    REDISMODULE_GET_API(CreateDataType);
    REDISMODULE_GET_API(ModuleTypeSetValue);
    REDISMODULE_GET_API(ModuleTypeGetType);
    REDISMODULE_GET_API(ModuleTypeGetValue);
    REDISMODULE_GET_API(SaveUnsigned);
    REDISMODULE_GET_API(LoadUnsigned);
    REDISMODULE_GET_API(SaveSigned);
    REDISMODULE_GET_API(LoadSigned);
    REDISMODULE_GET_API(SaveString);
    REDISMODULE_GET_API(SaveStringBuffer);
    REDISMODULE_GET_API(LoadString);
    REDISMODULE_GET_API(LoadStringBuffer);
    REDISMODULE_GET_API(SaveDouble);
    REDISMODULE_GET_API(LoadDouble);
    REDISMODULE_GET_API(SaveFloat);
    REDISMODULE_GET_API(LoadFloat);
    REDISMODULE_GET_API(EmitAOF);
    REDISMODULE_GET_API(Log);
    REDISMODULE_GET_API(LogIOError);
    REDISMODULE_GET_API(StringAppendBuffer);
    REDISMODULE_GET_API(RetainString);
    REDISMODULE_GET_API(StringCompare);
    REDISMODULE_GET_API(GetContextFromIO);
    REDISMODULE_GET_API(BlockClient);
    REDISMODULE_GET_API(UnblockClient);
    REDISMODULE_GET_API(IsBlockedReplyRequest);
    REDISMODULE_GET_API(IsBlockedTimeoutRequest);
    REDISMODULE_GET_API(GetBlockedClientPrivateData);
    REDISMODULE_GET_API(AbortBlock);
    REDISMODULE_GET_API(Milliseconds);
    REDISMODULE_GET_API(GetThreadSafeContext);
    REDISMODULE_GET_API(FreeThreadSafeContext);
    REDISMODULE_GET_API(ThreadSafeContextLock);
    REDISMODULE_GET_API(ThreadSafeContextUnlock);
    REDISMODULE_GET_API(DigestAddStringBuffer);
    REDISMODULE_GET_API(DigestAddLongLong);
    REDISMODULE_GET_API(DigestEndSequence);

    RedisModule_SetModuleAttribs(ctx,name,ver,apiver);
    return REDISMODULE_OK;
}

#else

/* Things only defined for the modules core, not exported to modules
 * including this file. */
#define RedisModuleString robj

#endif /* REDISMODULE_CORE */
#endif /* REDISMOUDLE_H */
