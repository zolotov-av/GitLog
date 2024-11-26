#pragma once

#include "common.h"
#include "oid.h"

namespace git
{

    class commit;

    /**
     * @brief Класс представляющий дерево коммита
     * @ingroup GitCXX
     *
     * Позволяет получить список файлов коммите и добраться
     * до их содержимого.
     */
    class tree
    {
    private:

        git_tree *m_tree { nullptr };

    public:

        class entry
        {
        private:

            git_tree_entry *m_entry { nullptr };

        public:

            entry() = default;
            entry(const entry &) = delete;
            entry(entry &&other): m_entry{ other.m_entry }
            {
                other.m_entry = nullptr;
            }
            entry(git_tree_entry *v): m_entry{ v }
            {
            }

            ~entry()
            {
                git_tree_entry_free(m_entry);
            }

            entry& operator = (const entry &) = delete;
            entry& operator = (entry &&other);

            object_id id() const
            {
                return object_id{ git_tree_entry_id(m_entry) };
            }

            QString name() const;

        };

        tree() = default;
        tree(const tree &) = delete;
        tree(tree &&other): m_tree(other.m_tree)
        {
        }
        tree(git_tree *v): m_tree(v)
        {
        }
        tree(const commit &commit);

        ~tree()
        {
            git_tree_free(m_tree);
        }

        tree& operator = (const tree &) = delete;
        tree& operator = (tree &&other);

        bool exists(const QString &path);
        entry entryByPath(const QString &path) const;

        git_tree* data() const
        {
            return m_tree;
        }

    };

}
