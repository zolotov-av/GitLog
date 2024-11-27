#pragma once

#include "common.h"

namespace git
{

    class signature
    {
    private:

        git_signature *m_sig { nullptr };

    public:

        explicit signature(git_signature *s = nullptr): m_sig{s} { }
        signature(const signature &) = delete;
        signature(signature &&other);
        ~signature() { git_signature_free(m_sig); }

        signature& operator = (const signature &) = delete;
        signature& operator = (signature &&other);

        static signature now(const QString &name, const QString &email);

        bool isNull() const { return m_sig == nullptr; }

        QString name() const { return QString::fromUtf8(m_sig->name); }
        QString email() const { return QString::fromUtf8(m_sig->email); }
        git_time when() const { return m_sig->when; }

        git_signature* data() { return m_sig; }
        const git_signature* data() const { return m_sig; }
        const git_signature* constData() const { return m_sig; }

    };

}
