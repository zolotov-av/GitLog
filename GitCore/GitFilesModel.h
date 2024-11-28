#pragma once

#include "GitRepository.h"
#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QList>

class GitFilesModel: public QAbstractItemModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(GitRepository* repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)

public:

    enum Roles
    {
        fileRole = Qt::DisplayRole
    };

    struct FileInfo
    {
        QString fileName { };
    };

private:

    GitRepository *m_repo { nullptr };
    git::reference m_ref { };
    QList<FileInfo> m_items { };

public:

    explicit GitFilesModel(QObject *parent = nullptr);
    GitFilesModel(const GitFilesModel &) = delete;
    GitFilesModel(GitFilesModel &&) = delete;
    ~GitFilesModel();

    GitFilesModel& operator = (const GitFilesModel &) = delete;
    GitFilesModel& operator = (GitFilesModel &&) = delete;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    GitRepository* repository() { return m_repo; }
    void setRepository(GitRepository *r);

    Q_INVOKABLE void update();

signals:

    void repositoryChanged();

};
