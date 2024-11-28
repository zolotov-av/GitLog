#include "GitFilesModel.h"
#include "awCore/trace.h"

GitFilesModel::GitFilesModel(QObject *parent): QAbstractItemModel{parent}
{

}

GitFilesModel::~GitFilesModel()
{

}

int GitFilesModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }

    return m_items.size();
}

int GitFilesModel::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }

    return 1;
}

QModelIndex GitFilesModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return createIndex(row, column, 1);
    }

    return QModelIndex{ };
}

QModelIndex GitFilesModel::parent(const QModelIndex &child) const
{
    return QModelIndex{ };
}

QVariant GitFilesModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() >= m_items.size() )
        return QVariant{ };

    switch (role )
    {
    case fileRole:
        return m_items.at(index.row()).fileName;
        return QVariant{ };
    default:
        return QVariant{ };
    }
}

QHash<int, QByteArray> GitFilesModel::roleNames() const
{
    return {
        {fileRole, "fileName"}
    };
}

void GitFilesModel::setRepository(GitRepository *r)
{
    if ( m_repo == r )
        return;

    m_repo = r;

    emit repositoryChanged();

    update();
}

void GitFilesModel::update()
{
    aw::trace fn("GitFilesModel::update()");

    if ( m_repo == nullptr || !m_repo->isOpened() )
    {
        aw::trace::log("GitFilesModel::update() not opened");

        beginResetModel();

        m_items.clear();

        endResetModel();
        return;
    }

    aw::trace::log("GitFilesModel::update() iterate...");
    beginResetModel();

    m_items.clear();

    auto iter = m_repo->newReferenceRterator();

    while ( true )
    {
        auto ref = iter.next();
        if ( ref.isNull() )
            break;

        m_items.append(FileInfo{ref.shortName()});
    }

    endResetModel();
}
