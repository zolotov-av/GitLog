#pragma once

#include "common.h"
#include "oid.h"

namespace git
{

    /**
     * @brief Класс представляющий одну из сторон дифа
     * @ingroup GitCXX
     */
    class diff_file
    {
    private:

        const git_diff_file *m_file { nullptr };

    public:

        constexpr diff_file() = default;
        constexpr diff_file(const diff_file &) = default;
        constexpr diff_file(diff_file &&) = default;
        diff_file(const git_diff_file *v): m_file{ v } { }

        diff_file& operator = (const diff_file &) = default;
        diff_file& operator = (diff_file &&) = default;

        QString path() const
        {
            return QString::fromUtf8(m_file->path);
        }

        object_id oid() const
        {
            return m_file->id;
        }

        const git_diff_file* data() { return m_file; }
    };

}
