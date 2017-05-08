#pragma once

#include <string>
#include "Core/Assert.h"
#include "hiredis-0.13.3/hiredis.h"

template<typename... Arg>
struct ReturnTypesCollection{
};

typedef ReturnTypesCollection<std::string, int> GeneralReturnTypesCollection;

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
    const static int typeLocation = 0;
};

template<typename X, typename T, typename... Arg>
struct VerifyDBReturnType<X, T, Arg...>
{
    const static int typeLocation = (VerifyDBReturnType<X, Arg...>::typeLocation >= 0 ? VerifyDBReturnType<X, Arg...>::typeLocation + 1 : VerifyDBReturnType<X, Arg...>::typeLocation);
};

template<typename... Arg>
struct VerifyHelper
{
};

template<typename... Arg>
struct VerifyHelper<ReturnTypesCollection<Arg...>>
{
	template<typename X>
	static int GetTypeID()
	{
		return VerifyDBReturnType<X, Arg...>::typeLocation;
	}
};

class DBClientReply
{
public:
	DBClientReply(const char* rawBuffer, int length, int typeID): m_rawBuffer(rawBuffer),
		m_length(length), m_typeID(typeID == REDIS_REPLY_INTEGER){}
	
	template<typename T>
	operator T() const
	{
		VERIFY(m_typeID == VerifyHelper<GeneralReturnTypesCollection>::GetTypeID<T>(), 
				"Requested type and handed type dosn't match");
		return m_typeID == REDIS_REPLY_INTEGER ? *(reinterpret_cast<T*>(m_rawBuffer)) :
			std::string(m_rawBuffer, m_length);
	}
private:
	const char* const m_rawBuffer;
	int m_length;
	int m_typeID;
};
