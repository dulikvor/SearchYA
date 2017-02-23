#pragma once

#include <string>
#include "Exception.h"

#define ENUMERATION(name) \
public: \
    name(Enumeration e):m_currentEnum(e){} \
    operator Enumeration(){return m_currentEnum;} \
	const std::string ToString() const;\
\
private: \
	struct EnumStringPair\
	{\
		Enumeration enumValue;\
		std::string enumStrName;\
	};\
\
private: \
    Enumeration m_currentEnum; \
	static EnumStringPair m_enumToString[];

#define ENUMERATION_NAMING_BEGIN(name)\
	name::EnumStringPair name::m_enumToString[]={

#define ENUMERATION_NAMING_END(name)\
	};\
	\
	const std::string name::ToString() const\
	{\
		int size = sizeof(m_enumToString) / sizeof(EnumStringPair);\
		for(int index = 0; index < size; index++)\
		{\
			if(m_enumToString[index].enumValue == m_currentEnum)\
				return m_enumToString[index].enumStrName;\
		}\
		throw core::Exception(SOURCE, "%s", "Not all enum values are covered");\
	}

