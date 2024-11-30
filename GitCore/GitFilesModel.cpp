#include "GitFilesModel.h"
#include <gitcxx/exception.h>
#include "awCore/trace.h"
#include <QDir>
#include <QFileInfo>

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

    if ( m_repo )
    {
        try
        {
            const auto ref = m_repo->lookupReference(m_ref_name);
            m_ref_short_name = ref.shortName();
            m_commit_title = m_repo->lookupCommit(ref).shortMessage();
        }
        catch(const std::exception &e)
        {
            aw::trace::log("GitFilesModel::setReferenceName() warn: %s", e.what());
            m_ref_short_name = ref;
            m_commit_title.clear();
        }
    }
    else
    {
        m_ref_short_name = ref;
    }

    emit referenceNameChanged();

    update();
}

void GitFilesModel::setFilePath(const QString &path)
{
    if ( m_file_path == path )
        return;

    m_file_path = path;

    emit filePathChanged();

    update();
}

int GitFilesModel::indexOf(const QString &file)
{
    const int count = m_items.size();
    for(int i = 0; i < count; i++)
    {
        if ( file == m_items.at(i).fileName )
            return i;
    }

    return -1;
}

namespace
{

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

    struct EntryInfo
    {
        QString fileName;
        QString fileType;
        bool isDir;

        EntryInfo(const QString &name, git_object_t type):
            fileName{name}, fileType{entryTypeName(type)}, isDir{type!=GIT_OBJECT_BLOB}
        {

        }

        EntryInfo(const git::tree::const_entry &entry): EntryInfo{entry.name(), entry.type()} { }

        static bool compare(const EntryInfo &a, const EntryInfo &b)
        {
            if ( a.isDir && b.isDir )
            {
                return QString::compare(a.fileName, b.fileName, Qt::CaseSensitive) < 0;
            }

            if ( a.isDir )
                return true;

            if ( b.isDir )
                return false;

            return QString::compare(a.fileName, b.fileName, Qt::CaseSensitive) < 0;
        }
    };

    static std::vector<EntryInfo> treeEntries(const git::tree &tree)
    {
        const size_t count = tree.entryCount();
        std::vector<EntryInfo> list;
        list.reserve(count);
        for(size_t i = 0; i < count; i++)
        {
            list.emplace_back(EntryInfo{tree.entryByIndex(i).constData()});
        }

        std::sort(list.begin(), list.end(), &EntryInfo::compare);

        return list;
    }

    static git::tree appyPath(GitRepository *repo, git::tree &&tree, const QString &path)
    {
        if ( path.isEmpty() || path == QChar('/') )
            return std::move(tree);

        if( path.at(0) != QChar{'/'} )
            return std::move(tree);

        QStringView rest = QStringView{ path }.mid(1);

        while ( true )
        {

            const auto pos = rest.indexOf(QChar('/'));
            if ( pos < 0 )
            {
                const auto entry = tree.entryByPath(rest.toString());
                if ( entry.type() != GIT_OBJECT_TREE )
                    throw git::exception("%s is not directory", path);

                return repo->lookupTree(entry.id());
            }

            const auto entry = tree.entryByPath(rest.mid(0, pos).toString());
            if ( entry.type() != GIT_OBJECT_TREE )
                throw git::exception("%s is not directory", path);

            tree = repo->lookupTree(entry.id());
            rest = rest.mid(pos + 1);
        }

        return std::move(tree);
    }
}

void GitFilesModel::readTree(const git::tree &tree)
{
    const auto entries = treeEntries(tree);
    m_items.reserve(entries.size() + 1);
    m_items.append(FileInfo{"..", "up"});
    for(const auto &item : entries)
    {
        m_items.append(FileInfo{item.fileName, item.fileType});
    }
}

void GitFilesModel::clear()
{
    beginResetModel();

    m_items.clear();

    endResetModel();
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

    try
    {
        const auto commit_id = m_repo->lookupReference(m_ref_name).resolve().target();
        const auto commit = m_repo->lookupCommit(commit_id);
        const auto tree = appyPath(m_repo, commit.commitTree(), m_file_path);

        readTree(tree);
    }
    catch(const std::exception &e)
    {
        aw::trace::log("GitFilesModel::update() error: %s", e.what());
    }

    endResetModel();
}

void GitFilesModel::enter(const QString &file)
{
    aw::trace::log("GitFilesModel::enter(%s)", file);
    try
    {
        const auto newPath = QDir{m_file_path}.filePath(file);
        const auto commit_id = m_repo->lookupReference(m_ref_name).resolve().target();
        const auto commit = m_repo->lookupCommit(commit_id);
        const auto tree = appyPath(m_repo, commit.commitTree(), newPath);

        m_file_path = newPath;
        aw::trace::log("GitFilesModel::enter() new path=%s", newPath);

        emit filePathChanged();
    }
    catch(const std::exception &e)
    {
        aw::trace::log("GitFilesModel::enter() error: %s", e.what());
    }

    update();
}

int GitFilesModel::leave()
{
    aw::trace::log("GitFilesModel::leave() current path=%s", m_file_path);

    if ( m_file_path == "/" || m_file_path.isEmpty() )
        return 0;

    const QFileInfo info{m_file_path};
    m_file_path = info.dir().path();
    aw::trace::log("GitFilesModel::leave() new path=%s", m_file_path);

    emit filePathChanged();

    update();

    return indexOf(info.fileName());
}
