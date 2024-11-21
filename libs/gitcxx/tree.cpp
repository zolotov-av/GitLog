#include "tree.h"

namespace git
{

    tree::entry& tree::entry::operator = (entry &&other)
    {
        git_tree_entry_free(m_entry);
        m_entry = other.m_entry;
        other.m_entry = nullptr;
        return *this;
    }

    QString tree::entry::name() const
    {
        return QString::fromUtf8(git_tree_entry_name(m_entry));
    }

    tree::tree(const commit &commit)
    {
        check( git_commit_tree(&m_tree, commit.data()) );
    }

    tree& tree::operator = (tree &&other)
    {
        git_tree_free(m_tree);
        m_tree = other.m_tree;
        other.m_tree = nullptr;
        return *this;
    }

    tree::entry tree::entryByPath(const QString &path) const
    {
        const auto utf8_path = path.toUtf8();
        git_tree_entry *ent { nullptr };
        check( git_tree_entry_bypath(&ent, m_tree, utf8_path.constData()) );
        return ent;
    }

}
