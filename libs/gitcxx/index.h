#pragma once

#include "common.h"
#include "oid.h"

namespace git
{

    /**
     * @brief Класс представляющий индекс
     * @ingroup GitCXX
     *
     * Позволяет читать проиндексированные данные
     */
    class index
    {
    private:

        git_index *m_index { nullptr };

    public:

        constexpr index() = default;
        index(const index &) = delete;
        index(index &&other): m_index(other.m_index)
        {
            other.m_index = nullptr;
        }
        index(git_index *idx): m_index(idx)
        {

        }

        ~index()
        {
            git_index_free(m_index);
        }

        index& operator = (const index &) = delete;
        index& operator = (index &&other);

        object_id oidByPath(const QString &path);

    };

}
