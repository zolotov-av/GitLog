#include "diff.h"

namespace git
{

    diff& diff::operator = (diff &&other)
    {
        git_diff_free(m_diff);
        m_diff = other.m_diff;
        other.m_diff = nullptr;
        return *this;
    }

}
