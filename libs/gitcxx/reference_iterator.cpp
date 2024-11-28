#include "reference_iterator.h"
#include "reference.h"

namespace git
{

    reference_iterator::reference_iterator(reference_iterator &&other): m_iter{other.m_iter}
    {
        other.m_iter = nullptr;
    }

    reference_iterator& reference_iterator::operator =(reference_iterator &&other)
    {
        git_reference_iterator_free(m_iter);
        m_iter = other.m_iter;
        other.m_iter = nullptr;
        return *this;
    }

    reference reference_iterator::next()
    {
        git_reference *ref { nullptr };
        const auto ret = git_reference_next(&ref, m_iter);
        if ( ret == GIT_ITEROVER )
            return reference{ nullptr };

        check( ret );
        return reference{ ref };
    }

    QString reference_iterator::nextName()
    {
        const char *refName { nullptr };
        const auto ret = git_reference_next_name(&refName, m_iter);
        if ( ret == GIT_ITEROVER )
            return QString{ };

        check( ret );
        return QString::fromUtf8(refName);
    }

}
