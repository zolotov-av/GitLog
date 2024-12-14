#include "GitDiffModel.h"
#include <QFile>
#include <QBuffer>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <gitcxx/exception.h>
#include <awCore/string.h>
#include <awCore/trace.h>

namespace
{

    struct PayloadData
    {
		/**
		 * @brief старый контент разбитый на строки
		 */
		GitDiffLine::LineList old_lines { };

		/**
		 * @brief новый контент разбитый на строки
		 */
		GitDiffLine::LineList new_lines { };

		/**
		 * @brief нормализованные старый контент
		 */
		QByteArray oldContent { GitDiffLine::joinLines(old_lines.lines).toUtf8() };

		/**
		 * @brief нормализованные новый контент
		 */
		QByteArray newContent { GitDiffLine::joinLines(new_lines.lines).toUtf8() };

		QList<GitDiffLine> items { };
		int left_start { 1 };
		int right_start { 1 };

		PayloadData(const QByteArray &oldData, const QByteArray &newData):
			old_lines{ GitDiffLine::splitLines(oldData) },
			new_lines{ GitDiffLine::splitLines(newData) }
		{

		}

		int oldLines() const { return old_lines.size(); }
		int newLines() const { return new_lines.size(); }

		QString oldLine(int lineNo) const { return old_lines.lines[lineNo-1]; }
		QString newLine(int lineNo) const { return new_lines.lines[lineNo-1]; }

        void append(const QString &line, QColor color)
        {
			GitDiffLine item;
			item.lineNumber = items.size() + 1;
            item.lineText = line;
            item.lineColor = color;
			items.append(std::move(item));
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

	QString joinLines(const QList<GitDiffLine> &items)
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
		const aw::trace fn("hunk_cb");
        const auto data = reinterpret_cast<PayloadData*>(payload);
		aw::trace::log("hunk: old %d..%d", hunk->old_start, hunk->old_start + hunk->old_lines-1);
		aw::trace::log("hunk: new %d..%d", hunk->new_start, hunk->new_start + hunk->new_lines-1);

        if ( hunk->old_start > data->left_start )
        {
			const int begin = data->left_start;
			const int end = hunk->old_start;
			aw::trace::log("hunk: add old lines %d..%d", begin, end-1);
			for(int i = begin; i < end; i++)
            {
				data->append(data->oldLine(i), Qt::transparent);
            }
        }

        data->left_start = hunk->old_start + hunk->old_lines;
        data->right_start = hunk->new_start + hunk->new_lines;

        return 0;
    }

    int line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload)
    {
		const aw::trace fn("line_cb");
        const auto data = reinterpret_cast<PayloadData*>(payload);
		const std::string origin { line->origin };
		aw::trace::log("origin [%s]", origin.c_str());
		aw::trace::log("old_lineno %d", line->old_lineno);
		aw::trace::log("new_lineno %d", line->new_lineno);
		aw::trace::log("num_lines %d", line->num_lines);

		switch( line->origin )
		{
		case GIT_DIFF_LINE_ADDITION:
			data->append(data->newLine(line->new_lineno), GitDiffLine::addedColor());
			break;
		case GIT_DIFF_LINE_DELETION:
			data->append(data->oldLine(line->old_lineno), GitDiffLine::removedColor());
			break;
		case GIT_DIFF_LINE_CONTEXT:
			data->append(data->oldLine(line->old_lineno), Qt::transparent);
			break;
		default:
			break;
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

	const auto &line = m_items[index.row()];

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
			GitDiffLine line;
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

    int lineNumber = 0;
    QByteArray data2 { data };
    QBuffer file(&data2);
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        while ( !file.atEnd() )
        {
			GitDiffLine line;
            line.lineNumber = ++lineNumber;
            line.lineText = file.readLine();
            if ( line.lineText.endsWith(QChar{'\n'}) )
                line.lineText = line.lineText.mid(0, line.lineText.length()-1);

            line.lineColor = QColor{"transparent"};
            m_items.append(std::move(line));
        }

        m_text = joinLines(m_items);
    }
    else
    {
        aw::trace::log("Could not open file: %s", file.errorString());
        m_text = aw::qt_printf("Could not open file: %s", file.errorString());
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

	GitDiffLine line;
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

	PayloadData payload { left, right };

	aw::trace::log("old lines %d", payload.oldLines());
	aw::trace::log("new lines %d", payload.newLines());

	const auto ret = git_diff_buffers(payload.oldContent.constData(), payload.oldContent.size(), nullptr,
									  payload.newContent.constData(), payload.newContent.size(), nullptr,
                                      nullptr, nullptr, nullptr, &hunk_cb, &line_cb, &payload);
    if ( ret != 0 )
    {

    }

	if ( ret == 0 )
	{
        if ( !left.isEmpty() )
        {
			const int begin	= payload.left_start;
			const int end = payload.oldLines() + 1;
			aw::trace::log("rest(left) add old lines %d..%d", begin, end-1);
			for(int i = begin; i < end; i++)
            {
				payload.append(payload.oldLine(i), Qt::transparent);
            }
        }
        else if ( !right.isEmpty() )
        {
			qDebug().noquote() << "rest(right): " << payload.right_start << "/" << payload.oldLines();
			const int begin	= payload.right_start;
			const int end = payload.newLines() + 1;
			aw::trace::log("rest(right) add new lines %d..%d", begin, end-1);
			for(int i = begin; i < end; i++)
            {
				payload.append(payload.newLine(i), Qt::transparent);
            }
        }

		m_items = payload.items;
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

void GitDiffModel::setHeadFile(const QString &file)
{
    aw::trace::log("GitDiffModel::setHeadFile() %s", file);

    const auto commit = m_repo->lookupCommit(m_repo->head().target());
    const auto blob = m_repo->lookupBlob( commit.commitTree().entryByPath(file).id() );

    setContent(blob.rawContent());
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

void GitDiffModel::setTheirsFile(const QString &file)
{
    aw::trace::log("GitDiffModel::setTheirsFile() %s", file);
    try
    {
        auto index = m_repo->getIndex();
        const auto conflict = index.getConflictEntry(file);

        aw::trace::log("GitDiffModel::setTheirsFile() ancestor: %s", conflict.ancestor.id().toString());
        aw::trace::log("GitDiffModel::setTheirsFile() our: %s", conflict.our.id().toString());
        aw::trace::log("GitDiffModel::setTheirsFile() their: %s", conflict.their.id().toString());

        const auto blob = m_repo->lookupBlob( conflict.their.id() );

        setContent(blob.rawContent());
    }
    catch(const std::exception &e)
    {
        aw::trace::log("GitDiffModel::setTheirsFile() error: %s", e.what());
        clear();
    }
}

void GitDiffModel::setPatchFile(const QString &file)
{
    const aw::trace fn("GitDiffModel::setPatchFile() %s", file);

    try
    {
        const QString stoppedShaPath = QDir{m_repo->gitdir()}.filePath("rebase-merge/stopped-sha");
        if ( !QFileInfo{stoppedShaPath}.isFile() )
            throw git::exception("rebase-merge/stopped-sha not found");

        const git::object_id stoppedId { QString::fromUtf8( readFromFile(stoppedShaPath) ).trimmed() };

        aw::trace::log("rebase-merge/stopped-sha = [%s]", stoppedId.toString());

        const auto stoppedCommit = m_repo->lookupCommit(stoppedId);
        if ( stoppedCommit.parentCount() > 1 )
            throw git::exception("stoppedCommit.parentCount() > 1");

        const auto stoppedParent = m_repo->lookupCommit(stoppedCommit.parentId(0));
        const auto diff = m_repo->diffCommitFile(stoppedParent, stoppedCommit, file);
        aw::trace::log("GitDiffModel::setPatchFile() deltaCount=%d", int(diff.deltaCount()));
        if ( diff.deltaCount() != 1 )
            throw git::exception("deltaCount() != 1");

        setGitDelta(m_repo->repo(), diff.get_delta(0), false);
    }
    catch(const std::exception &e)
    {
        aw::trace::log("GitDiffModel::setPatchFile() error: %s", e.what());
        clear();
    }
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
    if ( m_file_source == "HEAD" )
    {
        setHeadFile(m_file_path);
        return;
    }

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

    if ( m_file_source == "theirs" )
    {
        setTheirsFile(m_file_path);
        return;
    }

    if ( m_file_source == "patch" )
    {
        setPatchFile(m_file_path);
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
