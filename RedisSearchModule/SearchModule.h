extern "C"
{
	struct RedisModuleCtx;
	struct RedisModuleString;

	int AddDocument(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
	int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
}

