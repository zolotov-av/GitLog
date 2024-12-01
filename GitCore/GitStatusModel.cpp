#include "GitStatusModel.h"
#include <GitRepository.h>
#include <QColor>
#include <awCore/trace.h>

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
    case fileNameRole:
        return m_items.at(index.row()).fileName();
    case fileStatusRole:
        return m_items.at(index.row()).statusName();
    case fileColorRole:
        return m_items.at(index.row()).fileColor();
    case statusSourceRole:
        return m_items.at(index.row()).statusSource();
    default:
        return QVariant{ };
    }
}

QHash<int, QByteArray> GitStatusModel::roleNames() const
{
    return {
        {fileNameRole, "fileName"},
        {fileColorRole, "fileColor"},
        {fileStatusRole, "fileStatus"},
        {statusSourceRole, "statusSource"}
    };
}

void GitStatusModel::setRepository(GitRepository *r)
{
    if ( m_repo == r )
        return;

    if ( m_repo )
        disconnect(m_repo, &GitRepository::stateChanged, this, &GitStatusModel::update);

    m_repo = r;

    if ( m_repo )
        connect(m_repo, &GitRepository::stateChanged, this, &GitStatusModel::update);

    emit repositoryChanged();

    update();
}

void GitStatusModel::update()
{
    if ( m_repo == nullptr || !m_repo->isOpened() )
    {
        beginResetModel();

        m_items.clear();

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

    unsigned deltaCount = diffCached.deltaCount();
    for(unsigned i = 0; i < deltaCount; i++)
    {
        const auto delta = diffCached.get_delta(i);
        if ( delta.type() == GIT_DELTA_CONFLICTED )
            continue;

        m_items.append(GitFileStatus{GitFileStatus::Stage, delta});
    }

    deltaCount = diffWorktree.deltaCount();
    for(unsigned i = 0; i < deltaCount; i++)
    {
        const auto delta = diffWorktree.get_delta(i);
        if ( delta.type() == GIT_DELTA_UNTRACKED || delta.type() == GIT_DELTA_CONFLICTED )
            continue;

        m_items.append(GitFileStatus{GitFileStatus::Worktree, delta});
    }

    for(unsigned i = 0; i < deltaCount; i++)
    {
        const auto delta = diffWorktree.get_delta(i);
        if ( delta.type() != GIT_DELTA_CONFLICTED )
            continue;

        m_items.append(GitFileStatus{GitFileStatus::Conflict, delta});
    }

    for(unsigned i = 0; i < deltaCount; i++)
    {
        const auto delta = diffWorktree.get_delta(i);
        if ( delta.type() != GIT_DELTA_UNTRACKED )
            continue;

        m_items.append(GitFileStatus{GitFileStatus::Untracked, delta});
    }

    endResetModel();
}

void GitStatusModel::stageFile(const QString &file)
{
    aw::trace::log("GitStatusModel::stageFile() %s", file);

    if ( m_repo )
        m_repo->stageFile(file);
}

void GitStatusModel::restoreStaged(const QString &file)
{
    aw::trace::log("GitStatusModel::restoreStaged() %s", file);

    if ( m_repo )
        m_repo->restoreStaged(file);
}

void GitStatusModel::checkoutHead(const QString &file)
{
    aw::trace::log("GitStatusModel::checkoutHead() %s", file);

    if ( m_repo )
        m_repo->checkoutHead(file);
}

void GitStatusModel::removeFile(const QString &file)
{
    aw::trace::log("GitStatusModel::removeFile() %s", file);

    if ( m_repo )
        m_repo->removeFile(file);
}
