#pragma once

#include "export.h"
#include "string.h"

namespace aw
{

    AW_CORE_API void setLogStream(FILE *s);
    AW_CORE_API void pushLogIndent();
    AW_CORE_API void popLogIndent();

    AW_CORE_API void log(const char *message);

    inline void log(const std::string &message)
    {
        log(message.c_str());
    }

    inline void log(const any_string<std::string> &message)
    {
        log(message.value.c_str());
    }

    template <typename ...Args>
    inline void log(const char *fmt, const Args&... args)
    {
        log(std_printf(fmt, args...));
    }

}
