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

private:

    template <typename ...Args>
    void emitError(const char *fmt, const Args&... args);

public:

    const QString& path() const { return m_path; }
    void setPath(const QString &value);

    bool isOpened() const { return m_repo.isOpened(); }

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();

    git::repository* repo() { return &m_repo; }

    git::index getIndex() { return m_repo.getIndex(); }

    git::reference head() { return m_repo.head(); }

    git::reference::iterator newReferenceRterator() { return m_repo.newReferenceRterator(); }

    git::revwalk newRevwalk() { return m_repo.newRevwalk(); }

    git::reference lookupReference(const QString &refName) { return m_repo.lookupReference(refName); }
    git::reference lookupReferenceDwim(const QString &shortName) { return m_repo.lookupReferenceDwim(shortName); }

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
    Q_INVOKABLE void stageAll(const QString &path, unsigned flags = GIT_INDEX_ADD_DEFAULT);

    /**
     * @brief Добавить файл в индекс
     * @param file путь к файлу (относительно рабочего каталога)
     *
     * Аналог команды git add -- file
     */
    Q_INVOKABLE void stageFile(const QString &file);

    /**
     * @brief Восстановить файл в индексе из HEAD
     * @param file путь к файлу (относительно рабочего каталога)
     *
     * Аналог команды git restore --staged file
     *
     * Восстанавливает только индекс, оставляя рабочий каталог как есть
     */
    Q_INVOKABLE void restoreStaged(const QString &file);

    /**
     * @brief Восстановить файл из HEAD
     * @param file путь к файлу (относительно рабочего каталога)
     *
     * Аналог команды git checkout HEAD -- file
     *
     * Обновляет индекс и рабочий каталог
     */
    Q_INVOKABLE void checkoutHead(const QString &file);

    /**
     * @brief Удалить файл в рабочем каталоге и в индексе
     * @param file путь к файлу (относительно рабочего каталога)
     *
     * Аналог команды git rm -- file
     *
     * Обновляет индекс и рабочий каталог
     */
    Q_INVOKABLE void removeFile(const QString &file);

    /**
     * @brief commit
     * @param message
     */
    Q_INVOKABLE void commit(const QString &message, bool amend = false);

    /**
     * @brief Исправить коммит
     * @param message новое сообщение
     *
     * Аналог команды git commit --amend -m message
     */
    Q_INVOKABLE void amend(const QString &message);

signals:

    void pathChanged();
    void stateChanged();
    void errorOccurred(const QString &message);

};
