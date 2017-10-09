#pragma once

#include "Core/src/Enumeration.h"

class JobType {
public:
    enum Enumeration{
        Init,
        DocumentIndexing,
        GetTopK
    };
    ENUMERATION(JobType);
};
