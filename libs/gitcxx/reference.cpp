#include "reference.h"

namespace git
{

    reference& reference::operator = (reference &&other)
    {
        git_reference_free(m_ref);
        m_ref = other.m_ref;
        other.m_ref = nullptr;
        return *this;
    }

    QString reference::name() const
    {
        const auto refName = git_reference_name(m_ref);
        return QString::fromUtf8(refName, qstrlen(refName));
    }

    QString reference::shortName() const
    {
        const auto refName = git_reference_shorthand(m_ref);
        return QString::fromUtf8(refName, qstrlen(refName));
    }

    reference reference::resolve() const
    {
        git_reference *out;
        check( git_reference_resolve(&out, m_ref) );
        return reference(out);
    }

    void reference::deleteReference()
    {
        check( git_reference_delete(m_ref) );
        free();
    }

    void reference::deleteBranch()
    {
        check( git_branch_delete(m_ref) );
        free();
    }

    void reference::free()
    {
        git_reference_free(m_ref);
        m_ref = nullptr;
    }

}
