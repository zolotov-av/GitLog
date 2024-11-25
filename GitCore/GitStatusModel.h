#pragma once

#include "GitFileInfo.h"
#include <gitcxx/repository.h>
#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QList>

class GitRepository;

class GitStatusModel: public QAbstractItemModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GitRepository* repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)

public:

    enum Roles
    {
        fileRole = Qt::DisplayRole,
        colorRole = Qt::ForegroundRole,
        statusRole = Qt::UserRole
    };

private:

    GitRepository *m_repo { nullptr };

    QList<GitFileInfo> m_items;
    QList<QColor> m_colors;

public:

    explicit GitStatusModel(QObject *parent = nullptr);
    GitStatusModel(const GitStatusModel &) = delete;
    GitStatusModel(GitStatusModel &&) = delete;
    ~GitStatusModel();

    GitStatusModel& operator = (const GitStatusModel &) = delete;
    GitStatusModel& operator = (GitStatusModel &&) = delete;

public:

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
