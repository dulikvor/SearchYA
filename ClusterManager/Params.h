#pragma once

#include <string>
#include <list>
#include <algorithm>
#include "Core/Assert.h"
#include "Core/Exception.h"
#include "Communication/GeneretedFiles/ClusterService.pb.h"
#include "Param.h"

class Params
{
public:
    Params(){}
    ~Params(){}


    void Load(const ClusterService::Params& params)
    {
        for(int index = 0; index < params.genericparams_size(); index++)
        {
            GeneralParam nativeParam;
            nativeParam.Load(params.genericparams(index));
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
            throw Core::Exception(SOURCE, "A requested key was already existed - %s", key.c_str());
        }
    }
	
    GeneralParam GetValue(const std::string& key) const
    {
        typedef std::pair<std::string, GeneralParam> ParamPair;
        auto comparator = [&key](const ParamPair& pair) -> bool {return pair.first == key;}; //redundancy from above, but never mind :)
        std::list<ParamPair>::const_iterator it = std::find_if(m_values.begin(), m_values.end(), comparator);
        ASSERT(it != m_values.end(), "Requested Key was not found - %s", key.c_str());
        return it->second;
    }

private:
    std::list<std::pair<std::string, GeneralParam>> m_values;
};

class StringConverter
{
public:
	static std::string Convert(const std::string str) { return str; }
};
