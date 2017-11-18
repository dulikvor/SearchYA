#pragma once

#include <string>
#include <list>
#include <algorithm>
#include "Core/src/Assert.h"
#include "Core/src/Exception.h"
#include "GeneretedFiles/TextualSearchService.pb.h"
#include "Param.h"
#include "Serializor.h"

using namespace core;

typedef Param<GeneralTypesCollection> GeneralParam;

class GeneralParams
{
public:
   GeneralParams(){}
    ~GeneralParams(){}


    void Load(const TextualSearchService::Params& params)
    {
        for(int index = 0; index < params.genericparams_size(); index++)
        {
            GeneralParam nativeParam;
            Load(params.genericparams(index), nativeParam);
            m_values.push_back({params.genericparams(index).name(), std::move(nativeParam)});
        }
    }

    template<typename X>
    void AddParam(const std::string& key, const X& value)
    {
        typedef std::pair<std::string, GeneralParam> ParamPair;
        auto comparator = [&key](const ParamPair& pair) -> bool {return pair.first == key;};
        if(std::find_if(m_values.begin(), m_values.end(), comparator) == m_values.end())
        {
            GeneralParam param;
            param.Set(value);
            m_values.push_back({key, std::move(param)});
        }
        else
        {
            throw core::Exception(SOURCE, "A requested key was already existed - %s", key.c_str());
        }
    }
	
    GeneralParam GetValue(const std::string& key) const
    {
        typedef std::pair<std::string, GeneralParam> ParamPair;
        auto comparator = [&key](const ParamPair& pair) -> bool {return pair.first == key;}; //redundancy from above, but never mind :)
        std::list<ParamPair>::const_iterator it = std::find_if(m_values.begin(), m_values.end(), comparator);
        if(it == m_values.end())
		   throw Exception(SOURCE, "Non existing parameter was requested %s", key.c_str());
        return it->second;
    }

	std::string Serialize(Serializor& serializeContext)
	{
		Serializor::Serialize(serializeContext, (int)m_values.size());
		for(const std::pair<std::string, GeneralParam>& param : m_values)
		{
			Serializor::Serialize(serializeContext, param.first);
			param.second.Serialize(serializeContext);
		}
		return serializeContext.ToString();
	}

	void Deserialize(Serializor& serializor)
	{
		int numOfValues = Serializor::DeserializeInt(serializor);
		for(int index = 0; index < numOfValues; index++)
		{
			std::string key = Serializor::DeserializeString(serializor);
			GeneralParam param(DeserializeParam(serializor));
			m_values.emplace_back(key, param);
		}
	}

private:

    void Load(const TextualSearchService::Param& param, GeneralParam& generalParam)
    {
		if(param.type() == TextualSearchService::Param::Primitive)
			SetPrimitive(param, generalParam);
		else //String list is the only second option as of now
			SetStringCollection(param, generalParam);
    }

	void SetPrimitive(const TextualSearchService::Param& param, GeneralParam& generalParam)
	{
		switch(param.valueOneOf_case())
		{
			case TextualSearchService::Param::ValueOneOfCase::kValueInt :
				{
					generalParam.Set(param.valueint());
					break;
				}
			case TextualSearchService::Param::ValueOneOfCase::kValueLong:
				{
					generalParam.Set(param.valuelong());
					break;
				}
			case TextualSearchService::Param::ValueOneOfCase::kValueBool:
				{
					generalParam.Set(param.valuebool());
					break;
				}
			case TextualSearchService::Param::ValueOneOfCase::kValueFloat:
				{
					generalParam.Set(param.valuefloat());
					break;
				}
			case TextualSearchService::Param::ValueOneOfCase::kValueDouble:
				{
					generalParam.Set(param.valuedouble());
					break;
				}
			case TextualSearchService::Param::ValueOneOfCase::kValueString:
				{
					generalParam.Set(param.valuestring());
					break;
				}
			defualt:
			{
				throw core::Exception(SOURCE, "None supported generic value type was provided");
			}
		}
	}

	void SetStringCollection(const TextualSearchService::Param& param, GeneralParam& generalParam)
	{
		std::list<std::string> stringCollection;
		for(int index = 0; index < param.valuestringcollection_size(); index++)
		{
			stringCollection.push_back(param.valuestringcollection(index));		
		}
		generalParam.Set(stringCollection);
	}

	GeneralParam DeserializeParam(Serializor& serializeContext)
	{
		int type = Serializor::DeserializeInt(serializeContext);
		GeneralParam param;
		if(type == VariantHelper<GeneralTypesCollection>::GetTypeID<bool>())
		{
			bool value = Serializor::DeserializeBool(serializeContext);
			param.Set(value);
		}
		else if(type == VariantHelper<GeneralTypesCollection>::GetTypeID<short>())
		{
			short value = Serializor::DeserializeShort(serializeContext);
			param.Set(value);
		}
		else if(type == VariantHelper<GeneralTypesCollection>::GetTypeID<int>())
		{
			int value = Serializor::DeserializeInt(serializeContext);
			param.Set(value);
		}
		else if(type == VariantHelper<GeneralTypesCollection>::GetTypeID<long>())
		{
			long value = Serializor::DeserializeLong(serializeContext);
			param.Set(value);
		}
		else if(type == VariantHelper<GeneralTypesCollection>::GetTypeID<float>())
		{
			float value = Serializor::DeserializeFloat(serializeContext);
			param.Set(value);
		}
		else if(type == VariantHelper<GeneralTypesCollection>::GetTypeID<double>())
		{
			double value = Serializor::DeserializeDouble(serializeContext);
			param.Set(value);
		}
		else if(type == VariantHelper<GeneralTypesCollection>::GetTypeID<std::string>())
		{
			std::string value = Serializor::DeserializeString(serializeContext);
			param.Set(value);
		}
		else if(type == VariantHelper<GeneralTypesCollection>::GetTypeID<std::list<std::string>>())
		{
			std::list<std::string> value = Serializor::DeserializeListString(serializeContext);
			param.Set(value);
		}
		return param;
	}
private:
    std::list<std::pair<std::string, GeneralParam>> m_values;
};

class StringConverter
{
public:
	static std::string Convert(const std::string str) { return str; }
};
