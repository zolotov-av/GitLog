#include "signature.h"

namespace git
{

    signature::signature(signature &&other): m_sig{other.m_sig}
    {
        other.m_sig = nullptr;
    }

    signature& signature::operator = (signature &&other)
    {
        git_signature_free(m_sig);
        m_sig = other.m_sig;
        other.m_sig = nullptr;
        return *this;
    }

    signature signature::now(const QString &name, const QString &email)
    {
        const auto utf8_name = name.toUtf8();
        const auto utf8_email = email.toUtf8();

        git_signature *sig { nullptr };
        check( git_signature_now(&sig, utf8_name.constData(), utf8_email.constData()) );

        return signature{ sig };
    }

}
