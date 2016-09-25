#pragma once

#include "GeneretedFiles/ClusterService.pb.h"
#include "Core/Enumeration.h"

class ParamValueType
{
public:
    ParamValueType()
    {
		static_assert(Primitive == (int)ClusterService::Param_ValueType::Param_ValueType_Primitive, "ParamValueType not sync with protobuf equivilant");
		static_assert(StringCollection == (int)ClusterService::Param_ValueType::Param_ValueType_StringCollection, "ParamValueType not sync with protobuf equivilant");
    }

    enum Enumeration
    {
		Primitive,
		StringCollection
    };

    ENUMERATION(ParamValueType)
};
