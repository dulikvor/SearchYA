#pragma once

#include <errno.h>
#include "Exception.h"
#include "LogDefs.h"

#define LINUX_VERIFY(expression) do{ if(!expression) throw Core::Exception(SOURCE, "An error occured, Reason - %s", strerror(errno)); } while(0)
#define ASSERT(expression, format, ...) do{ if(!(expression)) throw Core::Exception(SOURCE, format, ##__VA_ARGS__);} while(0)
