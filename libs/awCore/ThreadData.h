#pragma once

#include "string.h"

namespace aw
{

    class ThreadData
    {
    private:

        std::string m_name;

    public:

        const char* name() const { return m_name.c_str(); }

        void setName(const any_string<std::string> &name)
        {
            m_name = name.value;
        }

        ThreadData(const std::string &aName): m_name(aName)
        {
        }

    };

}
