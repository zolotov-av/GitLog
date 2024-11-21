#pragma once

#include "common.h"

namespace git
{

    /**
     * @brief Класс представляющий итератор коммитов
     * @ingroup GitCXX
     *
     * Позволяет прочитать все коммиты репозитария
     * или историю/лог конкретных веток
     */
    class revwalk
    {
    private:

        git_revwalk *m_revwalk { nullptr };

    public:

        revwalk() = default;
        revwalk(const revwalk &) = delete;
        revwalk(revwalk &&other);

        revwalk(git_revwalk *v): m_revwalk(v)
        {
        }

        revwalk(repository &repo);

        ~revwalk()
        {
            git_revwalk_free(m_revwalk);
        }

        revwalk& operator = (revwalk &&other);

        void setSorting(unsigned int sortMode)
        {
            git_revwalk_sorting(m_revwalk, sortMode);
        }

        void push(const object_id &oid);

        void pushRef(const QString &refName);

        bool next(git_oid &oid);

    };

}
