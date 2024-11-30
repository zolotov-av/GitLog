#include "commit.h"
#include "repository.h"
#include <QRegularExpression>

namespace git
{

    commit::commit(commit &&other): m_commit{ other.m_commit }
    {
        other.m_commit = nullptr;
    }

    commit::commit(repository &repo, const object_id &commit_id): commit{ repo.lookupCommit(commit_id) }
    {

    }

    commit& commit::operator = (commit &&other)
    {
        git_commit_free(m_commit);
        m_commit = other.m_commit;
        other.m_commit = nullptr;
        return *this;
    }

    QString commit::message() const
    {
        return QString::fromUtf8(git_commit_message(m_commit));
    }

    QString commit::shortMessage(int maxLen) const
    {
        return message().left(maxLen).split(QRegularExpression{"(\\r|\\n)"}).first();
    }

    QDateTime commit::commitTime() const
    {
        return QDateTime::fromSecsSinceEpoch(git_commit_time(m_commit));
    }

    tree commit::commitTree() const
    {
        git_tree *commitTree { nullptr };
        check( git_commit_tree(&commitTree, m_commit) );
        return tree{commitTree};
    }

}
