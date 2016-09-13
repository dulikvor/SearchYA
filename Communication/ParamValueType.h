#pragma once

#include "GeneretedFiles/ClusterService.pb.h"
#include "Core/Enumeration.h"

class ParamValueType
{
public:
    ParamValueType()
    {
		static_assert(Primitive == ClusterService::Param::Primitive, "ParamValueType not sync with protobuf equivilant");
		static_assert(StringCollection == ClusterService::Param::StringCollection, "ParamValueType not sync with protobuf equivilant");
    }

    enum Enumeration
    {
		Primitive,
		StringCollection
    };

    ENUMERATION(ParamValueType)
};
