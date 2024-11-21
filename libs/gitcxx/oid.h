#pragma once

#include "common.h"

namespace git
{

    /**
     * @brief Класс представляющий OID
     * @ingroup GitCXX
     */
    class object_id
    {
    private:

        git_oid m_oid { };

    public:

        constexpr object_id() = default;
        constexpr object_id(const object_id &) = default;
        constexpr object_id(object_id &&) = default;

        constexpr object_id(const git_oid &oid)
        {
            m_oid = oid;
        }

        constexpr object_id(const git_oid *oid)
        {
            if ( oid )
                m_oid = *oid;
        }

        explicit object_id(const QString &hash);

        object_id& operator = (const object_id &) = default;
        object_id& operator = (object_id &&) = default;
        object_id& operator = (const git_oid *oid);

        bool operator == (const object_id &o) const;
        bool operator != (const object_id &o) const;

        bool isNull() const
        {
            return git_oid_iszero(&m_oid);
        }

        const git_oid* data() const
        {
            return &m_oid;
        }

        QString toString() const;

    };

}
