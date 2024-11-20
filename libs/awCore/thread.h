#pragma once

#include "export.h"
#include "string.h"

namespace aw
{

    AW_CORE_API bool isCurrentThreadRegistered();
    AW_CORE_API const char* currentThreadName();
    AW_CORE_API void setCurrentThreadName(const any_string<std::string> &name);

}
