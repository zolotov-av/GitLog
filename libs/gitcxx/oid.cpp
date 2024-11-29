#include "oid.h"

namespace git
{

    object_id::object_id(const QString &hash)
    {
        const auto utf8_hash = hash.toUtf8();
        check( git_oid_fromstr(&m_oid, utf8_hash.constData()) );
    }

    object_id& object_id::operator = (const git_oid *oid)
    {
        if ( oid == nullptr )
        {
            m_oid = { };
            return *this;
        }

        m_oid = *oid;
        return *this;
    }

    bool object_id::operator == (const object_id &other) const
    {
        return compare(other) == 0;
    }

    bool object_id::operator != (const object_id &other) const
    {
        return compare(other) != 0;
    }

    QString object_id::toString() const
    {
        char buf[80];
        const auto ret = git_oid_tostr(buf, sizeof(buf), &m_oid);
        return QString::fromUtf8(ret);
    }

}
