#include "object.h"

namespace git
{

    object::object(object &&other): m_object{other.m_object}
    {
        other.m_object = nullptr;
    }

    object& object::operator = (object &&other)
    {
        git_object_free(m_object);
        m_object = other.m_object;
        other.m_object = nullptr;
        return *this;
    }

}
