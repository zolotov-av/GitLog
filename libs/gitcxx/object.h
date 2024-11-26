#pragma once

#include "common.h"

namespace git
{

    class object
    {
    private:

        git_object *m_object { nullptr };

    public:

        constexpr object(git_object *obj = nullptr): m_object{obj} { };
        object(const object &) = delete;
        object(object &&other);
        ~object() { git_object_free(m_object); }

        object& operator = (const object &) = delete;
        object& operator = (object &&other);

        git_object* data() { return m_object; }

    };

}
