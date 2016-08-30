#pragma once

#include <string>
#include <list>
#include <algorithm>
#include "Exception/Assert.h"
#include "Exception/Exception.h"
#include "Communication/GeneretedFiles/ClusterService.pb.h"
#include "Param.h"

typedef Param<short, int, long, float, double, bool, std::string> GeneralParam;

class Params
{
public:
    Params(){}
    ~Params(){}


    void Load(const ClusterService::Params& params)
    {
        GeneralParam nativeAlgoType;
        nativeAlgoType.Set(params.algotype());
        m_values.push_back({"Algo Type", std::move(nativeAlgoType)});
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
            throw Exception(SOURCE, "A requested key was already existed - %s", key.c_str());
        }
    }

    const GeneralParam& GetValue(const std::string& key)
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

