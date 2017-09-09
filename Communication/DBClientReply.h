#pragma once

#include <string>
#include <vector>
#include "Core/Assert.h"
#include "hiredis/hiredis.h"

template<typename... Arg>
struct ReturnTypesCollection{
};

typedef ReturnTypesCollection<std::string, std::vector<std::string>, int> GeneralReturnTypesCollection;

template<typename... Arg>
struct VerifyDBReturnType
{
};

template<typename X>
struct VerifyDBReturnType<X>
{
    const static int typeLocation = -1;
};

template<typename X, typename... Arg>
struct VerifyDBReturnType<X, X, Arg...>
{
    const static int typeLocation = 1;
};

template<typename X, typename T, typename... Arg>
struct VerifyDBReturnType<X, T, Arg...>
{
    const static int typeLocation = (VerifyDBReturnType<X, Arg...>::typeLocation >= 1 ? VerifyDBReturnType<X, Arg...>::typeLocation + 1 : VerifyDBReturnType<X, Arg...>::typeLocation);
};

template<typename... Arg>
struct VerifyHelper
{
};

template<typename... Arg>
struct VerifyHelper<ReturnTypesCollection<Arg...>>
{
	template<typename X>
	constexpr static int GetTypeID()
	{
		return VerifyDBReturnType<X, Arg...>::typeLocation;
	}
};

class DBClientReply
{
public:
	using BulkStringVector = std::vector<std::string>;
public:
	DBClientReply(const char* rawBuffer, int length, int typeID);
	DBClientReply(redisReply **replyNodes, int length, int typeID);
	~DBClientReply();
	
	template<typename T>
	operator T() const
	{
		VERIFY(m_typeID == VerifyHelper<GeneralReturnTypesCollection>::GetTypeID<T>(),
				"Requested type and handed type dosn't match");
		return m_typeID == REDIS_REPLY_INTEGER ? *(reinterpret_cast<T*>(m_rawBuffer.back().first)) :
			std::string(m_rawBuffer.back().first, m_rawBuffer.back().second);
	}

	operator BulkStringVector() const
	{
		VERIFY(m_typeID == VerifyHelper<GeneralReturnTypesCollection>::GetTypeID<BulkStringVector>(),
			   "Requested type and handed type dosn't match");
		BulkStringVector result;
		for(auto& buffer : m_rawBuffer)
			result.emplace_back(buffer.first, buffer.second);

		return result;
	}
private:
	std::vector<std::pair<char*, int>> m_rawBuffer;
	int m_typeID;
};
