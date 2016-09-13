#pragma once

#include <list>
#include <string>
#include <iostream>
#include "Core/Assert.h"
#include "Core/Exception.h"
#include "Core/LogDefs.h"
#include "Communication/GeneretedFiles/ClusterService.pb.h"
#include "Communication/ParamValueType.h"

template<typename... Arg>
struct VariantParam
{
};

template<typename X>
struct VariantParam<X>
{
    const static int typeLocation = -1;
};

template<typename X, typename... Arg>
struct VariantParam<X, X, Arg...>
{
    const static int typeLocation = 0;
};

template<typename X, typename T, typename... Arg>
struct VariantParam<X, T, Arg...>
{
    const static int typeLocation = (VariantParam<X, Arg...>::typeLocation >= 0 ? VariantParam<X, Arg...>::typeLocation + 1 : VariantParam<X, Arg...>::typeLocation);
};

template<typename ...Arg>
class Param
{
public:
    Param():m_rawBuffer(nullptr){}
    ~Param(){free(m_rawBuffer);}

    Param(const Param& obj) = delete;
    void operator = (const Param& rhs) = delete;

    void Load(const ClusterService::Param& param)
    {
		if(param.type() == ClusterService::Param::Primitive)
		{
			SetPrimitive(param);
		}
		else //String list is the only second option as of now
		{
			SetStringCollection(param);
		}
    }

    Param(Param&& obj)
    {
        free(m_rawBuffer);
        m_typeID = obj.GetTypeID();
        m_rawBuffer = obj.ReleaseBuffer();
    }

    template<typename X>
    void Set(const X& value)
    {
        m_typeID = VariantParam<X, Arg...>::typeLocation;
        m_rawBuffer = (char*)malloc(sizeof(char)*sizeof(X));
        new (m_rawBuffer) X(value);
    }
	
	template <typename X>
	void Set(const std::list<X>& value)
	{
		m_typeID = VariantParam<std::list<X>, Arg...>::typeLocation;
		m_rawBuffer = (char*)malloc(sizeof(std::list<X>)*sizeof(char));
		new (m_rawBuffer) std::list<X>(value);
	}
    template<typename X>
    operator X() const
    {
        int neededLocation = VariantParam<X, Arg...>::typeLocation;
        ASSERT(neededLocation == m_typeID, "Invalid implicit conversion was requested");
        return *(reinterpret_cast<X*>(m_rawBuffer));
    }

    //Accessor
    char* ReleaseBuffer()
    {
        char* temporal = nullptr;
        std::swap(temporal, m_rawBuffer);
        return temporal;
    }

    int GetTypeID() const {return m_typeID;}

private:
	void SetPrimitive(const ClusterService::Param& param)
	{
		switch(param.valueOneOf_case())
		{
			case ClusterService::Param::ValueOneOfCase::kValueInt :
				{
					Set(param.valueint());
					break;
				}
			case ClusterService::Param::ValueOneOfCase::kValueLong:
				{
					Set(param.valuelong());
					break;
				}
			case ClusterService::Param::ValueOneOfCase::kValueBool:
				{
					Set(param.valuebool());
					break;
				}
			case ClusterService::Param::ValueOneOfCase::kValueFloat:
				{
					Set(param.valuefloat());
					break;
				}
			case ClusterService::Param::ValueOneOfCase::kValueDouble:
				{
					Set(param.valuedouble());
					break;
				}
			case ClusterService::Param::ValueOneOfCase::kValueString:
				{
					Set(param.valuestring());
					break;
				}
		}
		defualt:
		{
			throw Exception(SOURCE, "None supported generic value type was provided");
		}
	}

	void SetStringCollection(const ClusterService::Param& param)
	{
		std::list<std::string> stringCollection;
		for(int index = 0; index < param.valuestringcollection_size(); index++)
		{
			stringCollection.push_back(param.valuestringcollection(index));		
		}
		Set(stringCollection);
	}

private:
    char* m_rawBuffer;
    int m_typeID;
};
