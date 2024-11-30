#pragma once

#include "common.h"
#include "oid.h"
#include "signature.h"
#include "SignatureInfo.h"
#include "tree.h"
#include <QDateTime>

namespace git
{

    /**
     * @brief Класс представляющий коммит
     * @ingroup GitCXX
     *
     * Позволяет получать информацию о коммите, такую
     * как сообщение, автора, дерево файлов и т.п.
     */
    class commit
    {
    private:

        git_commit *m_commit { nullptr };

    public:

        commit() = default;
        commit(const commit &) = delete;
        commit(commit &&other);

        commit(git_commit *v): m_commit(v)
        {
        }

        commit(repository &repo, const object_id &commit_id);

        ~commit()
        {
            git_commit_free(m_commit);
        }

        commit& operator = (const commit &) = delete;
        commit& operator = (commit &&other);

        bool isEmpty() const
        {
            return m_commit == nullptr;
        }

        object_id id() const
        {
            return git_commit_id(m_commit);
        }

        SignatureInfo author() const
        {
            return git_commit_author(m_commit);
        }

        QString message() const;
        QString shortMessage(int maxLen = 80) const;
        QDateTime commitTime() const;

        unsigned parentCount() const
        {
            return git_commit_parentcount(m_commit);
        }

        object_id parentId(unsigned n) const
        {
            return git_commit_parent_id(m_commit, n);
        }

        tree commitTree() const;

        git_commit* data() { return m_commit; }
        const git_commit* data() const { return m_commit; }
        const git_commit* constData() const { return m_commit; }

    };

}
