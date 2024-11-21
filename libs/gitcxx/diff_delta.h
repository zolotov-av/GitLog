#pragma once

#include "common.h"
#include "diff_file.h"

namespace git
{

    /**
     * @brief Класс представляющий дельту изменений
     * @ingroup GitCXX
     */
    class diff_delta
    {
    private:

        const git_diff_delta *m_delta { nullptr };

    public:

        diff_delta() = default;
        diff_delta(const diff_delta &) = default;
        diff_delta(diff_delta &&) = default;
        diff_delta(const git_diff_delta *v): m_delta{ v } { }

        diff_delta& operator = (const diff_delta &) = default;
        diff_delta& operator = (diff_delta &&) = default;

        git_delta_t type() const
        {
            return m_delta->status;
        }

        diff_file oldFile() const
        {
            return &m_delta->old_file;
        }

        diff_file newFile() const
        {
            return &m_delta->new_file;
        }

        const git_diff_delta* data() const { return m_delta; }
    };

}
