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
    Q_PROPERTY(QString referenceName READ referenceName WRITE setReferenceName NOTIFY referenceNameChanged FINAL)
    Q_PROPERTY(QString referenceShortName READ referenceShortName NOTIFY referenceNameChanged FINAL)
    Q_PROPERTY(QString commitTitle READ commitTitle NOTIFY referenceNameChanged FINAL)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged FINAL)

public:

    enum Roles
    {
        fileRole = Qt::DisplayRole,
        fileTypeRole = Qt::UserRole
    };

    struct FileInfo
    {
        QString fileName { };
        QString fileType { };
    };

private:

    GitRepository *m_repo { nullptr };
    QString m_ref_name { };
    QString m_ref_short_name { };
    QString m_commit_title { };
    QString m_file_path { };
    QVector<FileInfo> m_items { };

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

    const QString& referenceName() const { return m_ref_name; }
    const QString& referenceShortName() const { return m_ref_short_name; }
    const QString& commitTitle() const { return m_commit_title; }
    void setReferenceName(const QString &ref);

    const QString& filePath() const { return m_file_path; }
    void setFilePath(const QString &path);

    Q_INVOKABLE int indexOf(const QString &file);

private:

    void readTree(const git::tree &tree);

public slots:

    void clear();
    void update();

    /**
     * @brief Войти в подкаталог
     * @param file имя подкаталога
     *
     * Если это файл, то ничего не делать
     */
    void enter(const QString &file);

    /**
     * @brief Выйти из каталога
     * @return номер строки каталога
     *
     * Если это корневой каталог, то ничего не делать
     */
    Q_INVOKABLE int leave();

signals:

    void repositoryChanged();
    void referenceNameChanged();
    void filePathChanged();

};
