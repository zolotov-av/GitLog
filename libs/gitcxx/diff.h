#pragma once

#include "common.h"
#include "diff_delta.h"

namespace git
{

    /**
     * @brief Класс представляющий изменения (git diff)
     * @ingroup GitCXX
     */
    class diff
    {
    private:

        git_diff *m_diff { nullptr };

    public:

        diff() = default;
        diff(const diff &) = delete;
        diff(diff &&other): m_diff{ other.m_diff }
        {
            other.m_diff = nullptr;
        }
        diff(git_diff *v): m_diff{ v } { }

        ~diff()
        {
            git_diff_free(m_diff);
        }

        diff& operator = (const diff &) = delete;
        diff& operator = (diff &&other);

        unsigned deltaCount() const
        {
            return git_diff_num_deltas(m_diff);
        }

        diff_delta get_delta(unsigned n) const
        {
            return git_diff_get_delta(m_diff, n);
        }

    };

}
