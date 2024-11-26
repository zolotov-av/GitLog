#include "GitStatus.h"
#include <QDir>
#include <awCore/trace.h>

GitStatus::GitStatus(QObject *parent): QObject{parent}
{
    m_repo.setPath(QDir{QDir::homePath()}.filePath("prj/GitTools"));
    m_repo.open();
    m_model.setRepository(&m_repo);
}

GitStatus::~GitStatus()
{

}

void GitStatus::stageFile(const QString &file)
{
    aw::trace::log("GitStatus::stageFile() %s", file);
}
