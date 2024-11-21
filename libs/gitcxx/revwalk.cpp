#include "revwalk.h"
#include "exception.h"
#include "oid.h"
#include "repository.h"

namespace git
{

    revwalk::revwalk(revwalk &&other): m_revwalk(other.m_revwalk)
    {
        other.m_revwalk = nullptr;
    }

    revwalk::revwalk(repository &repo): revwalk{ repo.newRevwalk() }
    {

    }

    revwalk& revwalk::operator = (revwalk &&other)
    {
        git_revwalk_free(m_revwalk);
        m_revwalk = other.m_revwalk;
        other.m_revwalk = nullptr;
        return *this;
    }

    void revwalk::push(const object_id &oid)
    {
        check( git_revwalk_push(m_revwalk, oid.data()) );
    }

    void revwalk::pushRef(const QString &refName)
    {
        const auto utf8_name = refName.toUtf8();
        check( git_revwalk_push_ref(m_revwalk, utf8_name.constData()) );
    }

    bool revwalk::next(git_oid &oid)
    {
        const int ret = git_revwalk_next(&oid, m_revwalk);
        if ( ret == 0 )
            return true;

        if ( ret == GIT_ITEROVER )
            return false;

        throw exception(lastGitError());
    }

}
