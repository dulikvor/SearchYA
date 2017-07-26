#pragma once

#include "Core/Enumeration.h"

class JobType {
public:
    enum Enumeration{
        Init,
        DocumentIndexing,
        GetTopK
    };
    ENUMERATION(JobType);
};
