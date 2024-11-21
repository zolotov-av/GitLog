#pragma once

#include "common.h"
#include "oid.h"
#include "blob.h"
#include "commit.h"
#include "tree.h"
#include "diff.h"
#include "revwalk.h"
#include "config.h"
#include "index.h"
#include "reference.h"

namespace git
{

    /**
     * @brief Репозитарий Git
     * @ingroup GitCXX
     *
     * Главный класс библиотеки. Прежде чем работать с большей
     * частью API надо сначала открыть репозитарий.
     *
     * @note На данный момент пользователь библиотеки должен
     *   сам инициализировать библиотеку libgit2 вызвав
     *   git_libgit2_init().
     */
    class repository
    {
    private:

        git_repository *m_repo { nullptr };

    public:

        repository() = default;
        repository(const repository &) = delete;
        repository(repository &&other);
        repository(const QString &path);

        ~repository();

        repository& operator = (const repository &) = delete;
        repository& operator = (repository &&other);

        bool isOpened() const
        {
            return m_repo != nullptr;
        }

        void open(const QString &path);

        void close();

        config getConfig();

        index getIndex();

        QString workdir();

        reference head();

        reference lookupBranch(const QString &name, git_branch_t type = GIT_BRANCH_LOCAL);

        reference lookupReference(const QString &name);

        blob lookupBlob(const git_oid *id);
        blob lookupBlob(const object_id &oid)
        {
            return lookupBlob(oid.data());
        }

        commit lookupCommit(const git_oid *id);
        commit lookupCommit(const object_id &oid)
        {
            return lookupCommit(oid.data());
        }

        commit lookupCommit(const QString &hash);

        revwalk newRevwalk();

        reference createBranch(const QString &name, const git::commit &target, bool force = false);

        void deleteBranch(reference &&ref);

        void deleteBranch(const QString &name);

        void deleteRefence(reference &&ref);

        git::diff diff();

        git::diff diff(const git::tree &a, const git::tree &b);

        git::diff diff_cached(const git::tree &a);

        void createCommit(const QString &author_name, const QString &author_email, const QString &message);

        git_repository* data() const
        {
            return m_repo;
        }

    };

}
