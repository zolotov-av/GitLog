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
}

void GitRepository::close()
{
    const aw::trace fn("GitRepository::close()");
    m_repo.close();
}

void GitRepository::commit(const QString &message)
{
    if ( message.isEmpty() ) {
        aw::trace::log("GitRepository::commit() error: message is empty");
        emit errorOccurred("GitRepository::commit() error: message is empty");
        return;
    }

    aw::trace::log("GitRepository::commit() message: %s", message);

    git::config cfg = m_repo.getConfig();
    const auto authorName = cfg.getString("user.name");
    const auto authorMail = cfg.getString("user.email");

    m_repo.createCommit(authorName, authorMail, message);
}
