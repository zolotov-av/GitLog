#pragma once

#include "common.h"

namespace git
{

    /**
     * @brief Класс представляющий автора коммита
     * @ingroup GitCXX
     */
    class SignatureInfo
    {
    private:

        QString m_name;
        QString m_mail;
        git_time m_when;

    public:

        SignatureInfo(const git_signature *s);
        SignatureInfo(const SignatureInfo &) = default;
        SignatureInfo(SignatureInfo &&) = default;
        ~SignatureInfo() = default;

        SignatureInfo& operator = (const SignatureInfo &) = default;
        SignatureInfo& operator = (SignatureInfo &&) = default;

        const QString& name() const { return m_name; }
        const QString& mail() const { return m_mail; }
        git_time when() const { return m_when; }

    };

}
