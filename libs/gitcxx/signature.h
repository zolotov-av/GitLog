#pragma once

#include "common.h"

namespace git
{

    /**
     * @brief Класс представляющий автора коммита
     * @ingroup GitCXX
     */
    class signature
    {
    private:

        QString m_name;
        QString m_mail;
        git_time m_when;

    public:

        signature(const git_signature *s):
            m_name{QString::fromUtf8(s->name)},
            m_mail{QString::fromUtf8(s->email)},
            m_when(s->when)
        {
        }

        const QString& name() const { return m_name; }
        const QString& mail() const { return m_mail; }
        git_time when() const { return m_when; }

    };

}
