#pragma once

#include "ReferenceInfo.h"
#include "GitRepository.h"
#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QList>
#include <QColor>
#include <gitcxx/repository.h>

class GitRefsModel: public QAbstractItemModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(GitRepository* repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)

public:

    enum Roles
    {
        RefNameRole = Qt::DisplayRole,
        RefColorRole = Qt::BackgroundRole,
        RefTypeRole = Qt::UserRole
    };

    struct Item
    {
        QString refName;
        const char *refType;
        QColor refColor;
    };

private:

    GitRepository *m_repo { nullptr };
    QList<Item> m_refs;

public:

    explicit GitRefsModel(QObject *parent = nullptr);
    GitRefsModel(const GitRefsModel &other);
    GitRefsModel(GitRefsModel &&other);
    ~GitRefsModel();

    GitRefsModel& operator = (const GitRefsModel &other);
    GitRefsModel& operator = (GitRefsModel &&other);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    GitRepository* repository() { return m_repo; }
    void setRepository(GitRepository *r);

    void clear();
    void append(const ReferenceInfo &ref);

    void loadBranches(git::repository *repo);
    void loadTags(git::repository *repo);

public slots:

    void update();

signals:

    void repositoryChanged();

};
