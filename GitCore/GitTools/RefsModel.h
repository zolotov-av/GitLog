#pragma once

#include <QAbstractItemModel>
#include <QList>
#include <QColor>
#include <GitTools/reference_info.h>
#include <gitcxx/repository.h>

class RefsModel final: public QAbstractItemModel
{
    Q_OBJECT

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

    QList<Item> m_refs;

public:

    explicit RefsModel(QObject *parent = nullptr);
    RefsModel(const RefsModel &other);
    RefsModel(RefsModel &&other);
    ~RefsModel();

    RefsModel& operator = (const RefsModel &other);
    RefsModel& operator = (RefsModel &&other);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    void append(const git::reference_info &ref);

    void loadBranches(git::repository *repo);
    void loadTags(git::repository *repo);

};
