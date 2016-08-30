#pragma once

#include "GeneretedFiles/ClusterService.pb.h"
#include "Enumeration/Enumeration.h"

class ParamValueType
{
public:
    ParamValueType()
    {
        static_assert(BOOL == ClusterService::ParamValueType::BOOL, "ParamValueType not sync with protobuf equivilant");
        static_assert(INT == ClusterService::ParamValueType::INT, "ParamValueType not sync with protobuf equivilant");
        static_assert(LONG == ClusterService::ParamValueType::LONG, "ParamValueType not sync with protobuf equivilant");
        static_assert(FLOAT == ClusterService::ParamValueType::FLOAT, "ParamValueType not sync with protobuf equivilant");
        static_assert(DOUBLE == ClusterService::ParamValueType::DOUBLE, "ParamValueType not sync with protobuf equivilant");
        static_assert(STRING == ClusterService::ParamValueType::STRING, "ParamValueType not sync with protobuf equivilant");
    }

    enum Enumeration
    {
        BOOL,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE,
        STRING
    };

    ENUMERATION(ParamValueType)
};
