#include "GitDiffModel.h"
#include <QFile>
#include <QBuffer>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <awCore/trace.h>

namespace
{

    struct PayloadData
    {
        QList<GitDiffModel::LineInfo> *items;
        QStringList left;
        QStringList right;
        QColor addedColor;
        QColor removedColor;
        int left_start;
        int right_start;

        void append(const QString &line, QColor color)
        {
            GitDiffModel::LineInfo item;
            item.lineNumber = items->size() + 1;
            item.lineText = line;
            item.lineColor = color;
            items->append(std::move(item));
        }
    };

    QByteArray readFromFile(const QString &filePath)
    {
        QFile file{filePath};
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return { };
        }

        return file.readAll();
    }

    QStringList splitLines(const QByteArray &data)
    {
        QStringList items;

        QByteArray data2 { data };
        QBuffer file(&data2);
        if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            while ( !file.atEnd() )
            {
                QString line = QString::fromUtf8( file.readLine() );
                if ( line.endsWith(QChar{'\n'}) )
                    line = line.mid(0, line.length()-1);

                items.append(std::move(line));
            }

        }
        else
        {
            aw::trace::log("Could not open file: " + file.errorString());
        }

        return items;
    }

    QString joinLines(const QList<GitDiffModel::LineInfo> &items)
    {
        int size = items.size();
        for(const auto &item : items)
        {
            size += item.lineText.length();
        }

        QString text;
        text.reserve(size);

        for(const auto &item : items)
        {
            text.append(item.lineText).append(QChar('\n'));
        }

        return text;
    }

    int hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload)
    {
        const auto data = reinterpret_cast<PayloadData*>(payload);
        qDebug().noquote() << "old_start" << hunk->old_start << hunk->old_lines
                           << "new_start" << hunk->new_start << hunk->new_lines;

        if ( hunk->old_start > data->left_start )
        {
            for(int i = data->left_start; i <= hunk->old_start; i++)
            {
                data->append(data->left[i-1], Qt::transparent);
            }
        }

        data->left_start = hunk->old_start + hunk->old_lines;
        data->right_start = hunk->new_start + hunk->new_lines;

        return 0;
    }

    int line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload)
    {
        const auto data = reinterpret_cast<PayloadData*>(payload);

        if ( line->old_lineno == -1 )
        {
            data->append(data->right[line->new_lineno-1], data->addedColor);
        }
        else if ( line->new_lineno == -1 )
        {
            data->append(data->left[line->old_lineno-1], data->removedColor);
        }
        else
        {
            data->append(data->left[line->old_lineno-1], Qt::transparent);
        }

        return 0;
    }

} // namespace

GitDiffModel::GitDiffModel(QObject *parent): QAbstractListModel{parent}
{
    aw::trace::log("GitDiffModel create");
}

GitDiffModel::~GitDiffModel()
{
    aw::trace::log("GitDiffModel destroy");
}

QModelIndex GitDiffModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || row >= m_items.size() || column != 0 || parent.isValid()) {
        return QModelIndex();
    }

    return createIndex(row, column);
}

QModelIndex GitDiffModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GitDiffModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
        return 0;

    return m_items.size();
}

QVariant GitDiffModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return QVariant{};
    }

    const LineInfo &line = m_items[index.row()];

    switch (role)
    {
    case LineNumberRole:
        return line.lineNumber;
    case LineTextRole:
        return line.lineText;
    case LineColorRole:
        return line.lineColor;
    default:
        return QVariant{};
    }
}

QHash<int, QByteArray> GitDiffModel::roleNames() const
{
    return {
        {LineNumberRole, "lineNumber"},
        {LineTextRole, "lineText"},
        {LineColorRole, "lineColor"}
    };
}

void GitDiffModel::loadFromFile(const QString &path)
{
    beginResetModel();
    m_items.clear();

    int lineNumber = 0;
    QFile file{path};
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        while ( !file.atEnd() )
        {
            LineInfo line;
            line.lineNumber = ++lineNumber;
            line.lineText = file.readLine();
            if ( line.lineText.endsWith(QChar{'\n'}) )
                line.lineText = line.lineText.mid(0, line.lineText.length()-1);

            line.lineColor = QColor{"transparent"};
            m_items.append(std::move(line));
        }

    }
    else
    {
        aw::trace::log("Could not open file: %s", file.errorString());
    }

    file.close();

    if ( m_items.size() > 3 )
    {
        m_items[2].lineColor = QColor{"red"};
    }

    endResetModel();
}

void GitDiffModel::setContent(const QByteArray &data)
{
    beginResetModel();
    m_items.clear();

    m_text = QString::fromUtf8(data);

    int lineNumber = 0;
    QByteArray data2 { data };
    QBuffer file(&data2);
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        while ( !file.atEnd() )
        {
            LineInfo line;
            line.lineNumber = ++lineNumber;
            line.lineText = file.readLine();
            if ( line.lineText.endsWith(QChar{'\n'}) )
                line.lineText = line.lineText.mid(0, line.lineText.length()-1);

            line.lineColor = QColor{"transparent"};
            m_items.append(std::move(line));
        }

    }
    else
    {
        aw::trace::log("Could not open file: %s" + file.errorString());
    }

    endResetModel();

    emit textChanged();
}

void GitDiffModel::setLineColor(int row, QColor color)
{
    if ( row < 0 || row >= m_items.size() )
        return;

    m_items[row].lineColor = color;

    //const auto modelIndex = index(row);
    //emit dataChanged(modelIndex, modelIndex);
}

void GitDiffModel::clear()
{
    beginResetModel();
    m_items.clear();
    m_text.clear();
    endResetModel();
}

void GitDiffModel::addLine(const QString &text, QColor color)
{
    beginInsertRows({}, m_items.size(), m_items.size());

    LineInfo line;
    line.lineNumber = m_items.size() + 1;
    line.lineText = text;
    if ( line.lineText.endsWith(QChar{'\n'}) )
        line.lineText = line.lineText.mid(0, line.lineText.length()-1);

    m_items.append(std::move(line));

    endInsertRows();
}

void GitDiffModel::setDiffBuffers(const QByteArray &left, const QByteArray &right)
{
    beginResetModel();
    m_items.clear();

    PayloadData payload {
        &m_items,
        splitLines(left),
        splitLines(right),
        addedColor(),
        removedColor(),
        1, // left_start,
        1 // right_start
    };

    const auto ret = git_diff_buffers(left.constData(), left.size(), nullptr,
                                      right.constData(), right.size(), nullptr,
                                      nullptr, nullptr, nullptr, &hunk_cb, &line_cb, &payload);
    if ( ret != 0 )
    {

    }

    if ( ret == 0 )
    {
        if ( !left.isEmpty() )
        {

            qDebug().noquote() << "rest: " << payload.left_start << "/" << payload.left.size();
            for(int i = payload.left_start; i <= payload.left.size(); i++)
            {
                payload.append(payload.left[i-1], Qt::transparent);
            }
        }
        else if ( !right.isEmpty() )
        {
            qDebug().noquote() << "rest: " << payload.right_start << "/" << payload.right.size();
            for(int i = payload.right_start; i <= payload.right.size(); i++)
            {
                payload.append(payload.right[i-1], Qt::transparent);
            }
        }

        m_text = joinLines(m_items);
    }
    else
    {
        m_text = git::lastGitError();
        m_items.clear();
    }

    endResetModel();

    emit textChanged();
}

static QByteArray getData(git::repository *repo, const git::object_id &oid)
{
    if ( oid.isNull() )
        return { };

    const auto blob = repo->lookupBlob(oid.data());
    QByteArray data = blob.rawContent();
    data.detach();
    return data;
}

void GitDiffModel::setGitDelta(git::repository *repo, git::diff_delta delta, bool isWorktree)
{
    switch (delta.type())
    {
    case GIT_DELTA_IGNORED:
    case GIT_DELTA_UNTRACKED:
    {
        const QString path = QDir(repo->workdir()).filePath(delta.newFile().path());
        if ( QFileInfo(path).isFile() )
        {
            qDebug().noquote() << "GIT_DELTA_UNTRACKED";
            setDiffBuffers(QByteArray{}, readFromFile(path));
            return;
        }
        else
        {
            qDebug().noquote() << path << "is not file";
        }
        return;
    }
    default:
        break;
    }

    const auto oldFile = delta.oldFile();
    const auto newFile = delta.newFile();

    const auto oldOid = oldFile.oid();
    const auto newOid = newFile.oid();
    qDebug().noquote() << "old oid[" << oldOid.toString() << "] " << oldFile.path();
    qDebug().noquote() << "new oid[" << newOid.toString() << "] " << newFile.path();

    if ( isWorktree )
    {
        const QString path = QDir(repo->workdir()).filePath(newFile.path());
        qDebug().noquote() << "isWorktree path:" << path;

        const QByteArray oldData = getData(repo, oldOid);
        const QByteArray newData = readFromFile(path);
        setDiffBuffers(oldData, newData);
    }
    else
    {
        const QByteArray oldData = getData(repo, oldOid);
        const QByteArray newData = getData(repo, newOid);
        setDiffBuffers(oldData, newData);
    }

}

void GitDiffModel::setRepository(GitRepository *r)
{
    if ( m_repo == r )
        return;

    if ( m_repo )
        disconnect(m_repo, &GitRepository::stateChanged, this, &GitDiffModel::update);

    m_repo = r;

    if ( m_repo )
        connect(m_repo, &GitRepository::stateChanged, this, &GitDiffModel::update);

    emit repositoryChanged();
}

void GitDiffModel::setFilePath(const QString &path)
{
    if ( m_file_path == path )
        return;

    m_file_path = path;
    emit filePathChanged();
}

void GitDiffModel::setFileSource(const QString &src)
{
    if ( m_file_source == src )
        return;

    m_file_source = src;
    emit fileSourceChanged();
}

void GitDiffModel::setCachedFile(const QString &file)
{
    aw::trace::log("GitDiffModel::setCachedFile() %s", file);

    const auto commit = m_repo->lookupCommit(m_repo->head().target());
    const auto diff = m_repo->diffCachedFile(commit, file);
    aw::trace::log("GitDiffModel::setCachedFile() deltaCount=%d", int(diff.deltaCount()));
    if ( diff.deltaCount() == 1 )
    {
        setGitDelta(m_repo->repo(), diff.get_delta(0), false);
        return;
    }

    clear();
}

void GitDiffModel::setWorktreeFile(const QString &file)
{
    aw::trace::log("GitDiffModel::setWorktreeFile() %s", file);

    const auto diff = m_repo->diffWorktreeFile(file);
    aw::trace::log("GitDiffModel::setWorktreeFile() deltaCount=%d", int(diff.deltaCount()));
    if ( diff.deltaCount() == 1 )
    {
        setGitDelta(m_repo->repo(), diff.get_delta(0), false);
        return;
    }

    clear();
}

void GitDiffModel::setConflictedFile(const QString &file)
{
    aw::trace::log("GitDiffModel::setConflictedFile() %s", file);

    const QString path = QDir{m_repo->workdir()}.filePath(file);
    if ( !QFileInfo{path}.isFile() )
    {
        aw::trace::log("GitDiffModel::setUntrackedFile() error: not file");
        clear();
        return;
    }

    const auto commit = m_repo->lookupCommit(m_repo->head().target());
    const auto blob = m_repo->lookupBlob( commit.commitTree().entryByPath(file).id() );
    setDiffBuffers(blob.rawContent(), readFromFile(path));
}

void GitDiffModel::setUntrackedFile(const QString &file)
{
    aw::trace::log("GitDiffModel::setUntrackedFile() %s", file);

    const QString path = QDir{m_repo->workdir()}.filePath(file);
    if ( !QFileInfo{path}.isFile() )
    {
        aw::trace::log("GitDiffModel::setUntrackedFile() error: not file");
        clear();
        return;
    }

    setDiffBuffers(QByteArray{ }, readFromFile(path));
}

void GitDiffModel::update()
{
    if ( m_file_source == "stage" )
    {
        setCachedFile(m_file_path);
        return;
    }

    if ( m_file_source == "worktree" )
    {
        setWorktreeFile(m_file_path);
        return;
    }

    if ( m_file_source == "conflict" )
    {
        setConflictedFile(m_file_path);
        return;
    }

    if ( m_file_source == "untracked" )
    {
        setUntrackedFile(m_file_path);
        return;
    }

    aw::trace::log("GitDiffModel::update() %s @ %s", m_file_path, m_file_source);

    clear();
}
