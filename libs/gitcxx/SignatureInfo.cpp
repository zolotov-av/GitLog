#include "SignatureInfo.h"

namespace git
{

    SignatureInfo::SignatureInfo(const git_signature *s):
        m_name{ QString::fromUtf8(s->name) },
        m_mail{ QString::fromUtf8(s->email) },
        m_when{ s->when }
    {
    }

}
