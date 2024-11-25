#include "GitStatusModel.h"
#include <GitRepository.h>
#include <QColor>

GitStatusModel::GitStatusModel(QObject *parent): QAbstractItemModel{parent}
{

}

GitStatusModel::~GitStatusModel()
{

}

int GitStatusModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }

    return m_items.size();
}

int GitStatusModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex GitStatusModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return createIndex(row, column, 1);
    }

    return QModelIndex{ };
}

QModelIndex GitStatusModel::parent(const QModelIndex &child) const
{
    return QModelIndex{ };
}

QVariant GitStatusModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= m_items.size() )
        return QVariant{ };

    switch (role )
    {
    case fileRole:
        return m_items.at(index.row()).fileName();
    case statusRole:
        return m_items.at(index.row()).statusName();
    case colorRole:
        return m_colors.at(index.row());
    default:
        return QVariant{ };
    }
}

QHash<int, QByteArray> GitStatusModel::roleNames() const
{
    return {
        {fileRole, "fileName"},
        {colorRole, "fileColor"},
        {statusRole, "fileStatus"}
    };
}

void GitStatusModel::setRepository(GitRepository *r)
{
    if ( m_repo == r )
        return;

    m_repo = r;

    emit repositoryChanged();

    update();
}

void GitStatusModel::update()
{
    if ( m_repo == nullptr || !m_repo->isOpened() )
    {
        beginResetModel();

        m_items.clear();
        m_colors.clear();

        endResetModel();
        return;
    }

    beginResetModel();

    const auto repo = m_repo->repo();
    const auto commit = repo->lookupCommit(repo->head().target());
    const auto diffCached = repo->diff_cached(commit);
    const auto diffWorktree = repo->diff();

    const auto reserveSize = diffWorktree.deltaCount() + diffCached.deltaCount();
    m_items.clear();
    m_items.reserve(reserveSize);
    m_colors.clear();
    m_colors.reserve(reserveSize);

    constexpr QColor cachedColor{0x18, 0xb2, 0x18};
    constexpr QColor modifiedColor{0xb2, 0x18, 0x18};
    constexpr QColor untrackedColor{0x35, 0x35, 0x35};

    unsigned deltaCount = diffCached.deltaCount();
    for(unsigned i = 0; i < deltaCount; i++)
    {
        m_items.append(diffCached.get_delta(i));
        m_colors.append(cachedColor);
    }

    deltaCount = diffWorktree.deltaCount();
    for(unsigned i = 0; i < deltaCount; i++)
    {
        const auto delta = diffWorktree.get_delta(i);
        m_items.append(delta);
        m_colors.append( delta.type() == GIT_DELTA_UNTRACKED ? untrackedColor : modifiedColor );
    }

    endResetModel();
}
