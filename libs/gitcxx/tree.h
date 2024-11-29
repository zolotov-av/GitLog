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

        class const_entry
        {
        private:

            const git_tree_entry *m_entry { nullptr };

        public:

            constexpr const_entry(const git_tree_entry *ent = nullptr): m_entry{ent} { }
            const_entry(const const_entry &) = default;
            const_entry(const_entry &&) = default;
            ~const_entry() = default;

            const_entry& operator = (const const_entry &) = default;
            const_entry& operator = (const_entry &&) = default;

            constexpr bool isNull() const { return m_entry == nullptr; }

            object_id id() const
            {
                return object_id{ git_tree_entry_id(m_entry) };
            }

            QString name() const;
            git_object_t type() const { return git_tree_entry_type(m_entry); }
            QString typeName() const;

        };

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

        explicit tree(git_tree *v = nullptr): m_tree{v} { }
        tree(const tree &) = delete;
        tree(tree &&other): m_tree(other.m_tree)
        {
        }
        tree(const commit &commit);

        ~tree()
        {
            git_tree_free(m_tree);
        }

        tree& operator = (const tree &) = delete;
        tree& operator = (tree &&other);

        size_t entryCount() const
        {
            return git_tree_entrycount(m_tree);
        }

        const_entry entryByIndex(size_t index) const
        {
            return const_entry{ git_tree_entry_byindex(m_tree, index) };
        }

        bool exists(const QString &path);
        entry entryByPath(const QString &path) const;

        git_tree* data() const { return m_tree; }
        const git_tree* constData() const { return m_tree; }

    };

}
