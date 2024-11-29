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
    case fileTypeRole:
        return m_items.at(index.row()).fileType;
    default:
        return QVariant{ };
    }
}

QHash<int, QByteArray> GitFilesModel::roleNames() const
{
    return {
        {fileRole, "fileName"},
        {fileTypeRole, "fileType"}
    };
}

void GitFilesModel::setRepository(GitRepository *r)
{
    if ( m_repo == r )
        return;

    if ( m_repo )
        disconnect(m_repo, &GitRepository::stateChanged, this, &GitFilesModel::update);

    m_repo = r;

    if ( m_repo )
        connect(m_repo, &GitRepository::stateChanged, this, &GitFilesModel::update);

    emit repositoryChanged();

    update();
}

void GitFilesModel::setReferenceName(const QString &ref)
{
    if ( m_ref_name == ref )
        return;

    m_ref_name = ref;

    emit referenceNameChanged();

    update();
}

void GitFilesModel::clear()
{
    beginResetModel();

    m_items.clear();

    endResetModel();
}

static const char* entryTypeName(git_object_t fileType)
{
    switch(fileType)
    {
    case GIT_OBJECT_INVALID: return "invalid";
    case GIT_OBJECT_COMMIT: return "commit";
    case GIT_OBJECT_TREE: return "tree";
    case GIT_OBJECT_BLOB: return "file";
    case GIT_OBJECT_TAG: return "tag";
    case GIT_OBJECT_OFS_DELTA: return "ofs_delta";
    case GIT_OBJECT_REF_DELTA: return "ref_delta";
    default: return "unknown";
    }
}

void GitFilesModel::update()
{
    aw::trace fn("GitFilesModel::update()");

    if ( m_repo == nullptr || !m_repo->isOpened() || m_ref_name.isEmpty() )
    {
        aw::trace::log("GitFilesModel::update() not opened");

        clear();
        return;
    }

    aw::trace::log("GitFilesModel::update() iterate...");
    beginResetModel();

    m_items.clear();

    const auto commit_id = m_repo->lookupReference(m_ref_name).resolve().target();
    auto commit = m_repo->lookupCommit(commit_id);
    auto tree = commit.commitTree();

    const size_t count = tree.entryCount();
    for(size_t i = 0; i < count; i++)
    {
        const auto entry = tree.entryByIndex(i);
        const auto fileName = entry.name();
        const auto fileType = entryTypeName(entry.type());
        m_items.append(FileInfo{fileName, fileType});
    }

    endResetModel();
}
