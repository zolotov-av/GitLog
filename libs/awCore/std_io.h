#pragma once

#include "export.h"
#include "string.h"
#include <stdio.h>

namespace aw
{

    AW_CORE_API void atomic_put_line(FILE *stream, const char *message);

    inline void atomic_put_line(FILE *stream, const std::string &message)
    {
        atomic_put_line(stream, message.c_str());
    }

    inline void atomic_put_line(FILE *stream, const any_string<std::string> &message)
    {
        atomic_put_line(stream, message.value.c_str());
    }

    template <typename ...Args>
    void atomic_put_line(FILE *stream, const char *fmt, const Args&... args)
    {
        atomic_put_line(stream, std_printf(fmt, args...));
    }


    inline void std_out(const char *message)
    {
        atomic_put_line(stdout, message);
    }

    inline void std_out(const any_string<std::string> &message)
    {
        atomic_put_line(stdout, message.value.c_str());
    }

    template <typename ...Args>
    void std_out(const char *fmt, const Args&... args)
    {
        atomic_put_line(stdout, std_printf(fmt, args...));
    }

    inline void std_error(const char *message)
    {
        atomic_put_line(stderr, message);
    }

    inline void std_error(const any_string<std::string> &message)
    {
        atomic_put_line(stderr, message.value.c_str());
    }

    template <typename ...Args>
    void std_error(const char *fmt, const Args&... args)
    {
        atomic_put_line(stderr, std_printf(fmt, args...));
    }

}
