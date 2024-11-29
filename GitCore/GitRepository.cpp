#include "GitRepository.h"
#include <awCore/trace.h>

GitRepository::GitRepository(QObject *parent): QObject{parent}
{
    aw::trace::log("GitRepository create");
}

GitRepository::~GitRepository()
{
    aw::trace::log("GitRepository destroy");
}

template <typename ...Args>
void GitRepository::emitError(const char *fmt, const Args&... args)
{
    aw::trace::log(fmt, args...);
    emit errorOccurred(aw::qt_printf(fmt, args...));
}

void GitRepository::setPath(const QString &value)
{
    aw::trace::log("GitRepository::setPath(%s)", value);
    if ( m_path == value )
        return;

    m_path = value;
    emit pathChanged();
}

void GitRepository::open()
{
    const aw::trace fn("GitRepository::open()");
    m_repo.open(m_path);

    emit stateChanged();
}

void GitRepository::close()
{
    const aw::trace fn("GitRepository::close()");
    m_repo.close();

    emit stateChanged();
}

void GitRepository::stageAll(const QString &path, unsigned int flags)
{
    aw::trace::log("GitRepository::stageAll(%s)", path);

    if ( !m_repo.isOpened() )
        return;

    try
    {
        m_repo.stageAll(path, flags);
    }
    catch(const std::exception &e)
    {
        emitError("Stage error: %s", e.what());
    }

}

void GitRepository::stageFile(const QString &file)
{
    stageAll(file, GIT_INDEX_ADD_DISABLE_PATHSPEC_MATCH | GIT_INDEX_ADD_CHECK_PATHSPEC);
}

void GitRepository::restoreStaged(const QString &file)
{
    aw::trace::log("GitRepository::restoreStaged() %s", file);

    if ( !m_repo.isOpened() )
        return;

    try
    {
        m_repo.restoreStaged(file);
    }
    catch(const std::exception &e)
    {
        emitError("Restore staged error: %s", e.what());
    }
}

void GitRepository::checkoutHead(const QString &file)
{
    aw::trace::log("GitRepository::checkoutHead() %s", file);

    if ( !m_repo.isOpened() )
        return;

    try
    {
        auto commit = m_repo.lookupCommit(m_repo.head());
        if ( !commit.commitTree().exists(file) )
        {
            emitError("Checkout HEAD error: not found in HEAD: %s", file);
            return;
        }

        m_repo.checkoutHead(file);
    }
    catch(const std::exception &e)
    {
        emitError("Checkout HEAD error: %s", e.what());
    }
}

void GitRepository::removeFile(const QString &file)
{
    aw::trace::log("GitRepository::removeFile() %s", file);

    if ( !m_repo.isOpened() )
        return;

    try
    {
        m_repo.removeFile(file);
    }
    catch(const std::exception &e)
    {
        emitError("Remove error: %s", e.what());
    }
}

void GitRepository::commit(const QString &message, bool amend)
{
    if ( message.isEmpty() ) {
        aw::trace::log("GitRepository::commit() error: message is empty");
        emit errorOccurred("GitRepository::commit() error: message is empty");
        return;
    }

    aw::trace::log("GitRepository::commit() message: %s", message);

    if ( amend )
    {
        m_repo.amendCommit(message);
    }
    else
    {
        m_repo.createCommit(message);
    }
}

void GitRepository::amend(const QString &message)
{
    if ( message.isEmpty() ) {
        aw::trace::log("GitRepository::amend() error: message is empty");
        emit errorOccurred("GitRepository::amend() error: message is empty");
        return;
    }

    aw::trace::log("GitRepository::amend() message: %s", message);

    m_repo.amendCommit(message);
}
