#pragma once

#include "GitRepository.h"
#include "GitDiffLine.h"
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QColor>
#include <QList>
#include <gitcxx/repository.h>
#include <gitcxx/diff_delta.h>

/**
 * @brief Модель для подсветки строк в DiffArea
 * @ingroup GitCore
 */
class GitDiffModel: public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(GitRepository* repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    Q_PROPERTY(QString text READ text NOTIFY textChanged FINAL)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged FINAL)
    Q_PROPERTY(QString fileSource READ fileSource WRITE setFileSource NOTIFY fileSourceChanged FINAL)

public:

    enum Roles
    {
        LineTextRole = Qt::DisplayRole,
        LineColorRole = Qt::UserRole,
        LineNumberRole
    };

private:

    GitRepository *m_repo { nullptr };
    QString m_file_path { };
    QString m_file_source { };
	QList<GitDiffLine> m_items;
    QString m_text { };

public:

    explicit GitDiffModel(QObject *parent = nullptr);
    GitDiffModel(const GitDiffModel &) = delete;
    GitDiffModel(GitDiffModel &&) = delete;
    ~GitDiffModel();

    GitDiffModel& operator = (const GitDiffModel &) = delete;
    GitDiffModel& operator = (GitDiffModel &&) = delete;

    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void loadFromFile(const QString &path);
    void setContent(const QByteArray &data);

    void setLineColor(int row, QColor color);

    Q_INVOKABLE void clear();
    void addLine(const QString &text, QColor color);

    void setDiffBuffers(const QByteArray &left, const QByteArray &right);
    void setGitDelta(git::repository *repo, git::diff_delta delta, bool isWorktree);

    GitRepository* repository() { return m_repo; }
    void setRepository(GitRepository *r);

    const QString& filePath() const { return m_file_path; }
    void setFilePath(const QString &path);
    const QString& fileSource() const { return m_file_source; }
    void setFileSource(const QString &src);

    QString text() { return m_text; }

private:

    void setHeadFile(const QString &file);
    void setCachedFile(const QString &file);
    void setWorktreeFile(const QString &file);
    void setConflictedFile(const QString &file);
    void setTheirsFile(const QString &file);
    void setPatchFile(const QString &file);
    void setUntrackedFile(const QString &file);

public slots:

    void update();

signals:

    void repositoryChanged();
    void filePathChanged();
    void fileSourceChanged();
    void textChanged();

};
