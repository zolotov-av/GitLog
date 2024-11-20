#pragma once

#include "log.h"

namespace aw
{

    class trace
    {
    private:

        std::string m_message;

    public:

        template <typename ...Args>
        static void log(const Args&... args)
        {
            ::aw::log(args...);
        }

        template <typename ...Args>
        trace(const Args&... args): m_message(std_printf(args...))
        {
            ::aw::log("trace(%s) enter", m_message);
            ::aw::pushLogIndent();
        }

        ~trace()
        {
            ::aw::popLogIndent();
            ::aw::log("trace(%s) leave", m_message);
        }

    };

}
