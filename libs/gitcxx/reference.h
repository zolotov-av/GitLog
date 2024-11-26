#pragma once

#include "common.h"
#include "oid.h"
#include "object.h"

namespace git
{

    /**
     * @brief Класс представляющий ссылку (reference)
     * @ingroup GitCXX
     */
    class reference
    {
    private:

        git_reference *m_ref { nullptr };

    public:

        reference() = default;
        reference(const reference &other) = delete;
        reference(reference &&other): m_ref(other.m_ref)
        {
            other.m_ref = nullptr;
        }

        reference(git_reference *ref): m_ref(ref)
        {
        }

        ~reference()
        {
            free();
        }

        reference& operator = (const reference &) = delete;
        reference& operator = (reference &&other);

        bool isHead() const
        {
            return git_reference_is_branch(m_ref) != 0 && git_branch_is_head(m_ref) != 0;
        }

        bool isBranch() const
        {
            return git_reference_is_branch(m_ref);
        }

        bool isRemote() const
        {
            return git_reference_is_remote(m_ref);
        }

        bool isTag() const
        {
            return git_reference_is_tag(m_ref);
        }

        object_id target() const
        {
            return git_reference_target(m_ref);
        }

        QString name() const;
        QString shortName() const;

        reference resolve() const;
        object peel(git_object_t objectType = GIT_OBJECT_ANY);

        void deleteReference();
        void deleteBranch();
        void free();

    };

}
