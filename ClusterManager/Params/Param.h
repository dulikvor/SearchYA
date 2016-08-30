#pragma once

#include <iostream>
#include "Exception/Assert.h"
#include "Communication/GeneretedFiles/ClusterService.pb.h"
#include "Communication/DeserializeToNative/ParamValueType.h"

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
        switch(param.selectedparamvaluetype())
        {
        case ParamValueType::BOOL:
        {
            Set(param.valuebool());
            break;
        }
        case ParamValueType::DOUBLE:
        {
            Set(param.valuedouble());
            break;
        }
        case ParamValueType::FLOAT:
        {
            Set(param.valuefloat());
            break;
        }
        case ParamValueType::INT:
        {
            Set(param.valueint());
            break;
        }
        case ParamValueType::LONG:
        {
            Set(param.valuelong());
            break;
        }
        defualt:
        {

        }
        }
    }

    Param(Param&& obj)
    {
        free(m_rawBuffer);
        m_typeID = obj.GetTypeID();
        m_typeSize = obj.GetSize();
        m_rawBuffer = obj.ReleaseBuffer();
    }

    template<typename X>
    void Set(const X& value)
    {
        m_typeID = VariantParam<X, Arg...>::typeLocation;
        m_typeSize = sizeof(X);
        m_rawBuffer = (char*)malloc(sizeof(char)*m_typeSize);
        new (m_rawBuffer) X(value);
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
    int GetSize() const {return m_typeSize;}
private:
    char* m_rawBuffer;
    int m_typeID;
    int m_typeSize;
};
