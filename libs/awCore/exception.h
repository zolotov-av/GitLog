#pragma once

#include "export.h"
#include "string.h"
#include <exception>

namespace aw
{

    class AW_CORE_API exception: public std::exception
    {
    private:

        std::string m_message;

    public:

        exception(const any_string<std::string> &msg);
        exception(const exception &) = delete;
        exception(exception &&) = delete;

        template <typename ...Args>
        exception(const char *fmt, const Args&... args): exception(std_printf(fmt, args...))
        {

        }

        ~exception() override;

        exception& operator = (const exception &) = delete;
        exception& operator = (exception &&) = delete;

        const std::string& message() const { return m_message; }

        const char * what() const noexcept override;

    };

}
