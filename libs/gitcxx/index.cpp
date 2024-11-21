#include "index.h"

namespace git
{

    index& index::operator = (index &&other)
    {
        git_index_free(m_index);
        m_index = other.m_index;
        other.m_index = nullptr;
        return *this;
    }

    object_id index::oidByPath(const QString &path)
    {
        const auto u8_path = path.toUtf8();
        const auto entry = git_index_get_bypath(m_index, u8_path.constData(), 0);
        if (entry == nullptr)
            return { };

        return { entry->id };
    }

}
