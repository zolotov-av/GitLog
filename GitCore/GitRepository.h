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

    git::index getIndex() { return m_repo.getIndex(); }

    git::reference head() { return m_repo.head(); }

    git::commit lookupCommit(const git_oid *id) { return m_repo.lookupCommit(id); }
    git::commit lookupCommit(const git::object_id &oid) { return m_repo.lookupCommit(oid); }
    git::commit lookupCommit(const QString &hash) { return m_repo.lookupCommit(hash); }
    git::commit lookupCommit(const git::reference &ref) { return m_repo.lookupCommit(ref.resolve().target()); }

    /**
     * @brief Добавить файл/каталог в индекс
     * @param path путь к файлу (относительно рабочего каталога)
     *
     * Аналог команды git add -- path
     */
    void stageAll(const QString &path, unsigned flags = GIT_INDEX_ADD_DEFAULT) { m_repo.stageAll(path, flags); }

    /**
     * @brief Восстановить файл в индексе из HEAD
     * @param file путь к файлу (относительно рабочего каталога)
     *
     * Аналог команды git restore --staged file
     *
     * Восстанавливает только индекс, оставляя рабочий каталог как есть
     */
    void restoreStaged(const QString &file) { m_repo.restoreStaged(file); }

    /**
     * @brief Восстановить файл из HEAD
     * @param file путь к файлу (относительно рабочего каталога)
     *
     * Аналог команды git checkout HEAD -- file
     *
     * Обновляет индекс и рабочий каталог
     */
    void checkoutHead(const QString &file) { m_repo.checkoutHead(file); }

    /**
     * @brief Удалить файл в рабочем каталоге и в индексе
     * @param file путь к файлу (относительно рабочего каталога)
     *
     * Аналог команды git rm -- file
     *
     * Обновляет индекс и рабочий каталог
     */
    void removeFile(const QString &file) { m_repo.removeFile(file); }

    /**
     * @brief commit
     * @param message
     */
    Q_INVOKABLE void commit(const QString &message);

signals:

    void pathChanged();
    void errorOccurred(const QString &message);

};
