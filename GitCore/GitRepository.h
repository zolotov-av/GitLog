#pragma once

#include <QObject>
#include <QQmlEngine>
#include <gitcxx/repository.h>

class GitRepository: public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)

private:

    QString m_path;
    git::repository m_repo;

public:

    explicit GitRepository(QObject *parent = nullptr);
    GitRepository(const GitRepository &) = delete;
    GitRepository(GitRepository &&) = delete;
    ~GitRepository();

    GitRepository& operator = (const GitRepository &) = delete;
    GitRepository& operator = (GitRepository &&) = delete;

    const QString& path() const { return m_path; }
    void setPath(const QString &value);

    bool isOpened() const { return m_repo.isOpened(); }

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();

    git::repository* repo() { return &m_repo; }

signals:

    void pathChanged();

};
