#include "GitDiff.h"
#include <QFile>
#include <awCore/trace.h>

static QByteArray loadFromFile(const QString &filePath)
{
    QFile file{filePath};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        aw::trace::log("Could not open file: %s", file.errorString());
        return { };
    }

    return file.readAll();
}

GitDiff::GitDiff(QObject *parent): QObject{parent}
{
    aw::trace::log("GitDiff create");
}

GitDiff::~GitDiff()
{
    aw::trace::log("GitDiff destroy");
}

void GitDiff::diffData(const QByteArray &oldData, const QByteArray &newData)
{
    m_line_model.setDiffBuffers(oldData, newData);

    emit diffChanged();
}

void GitDiff::diffFiles(const QString &oldFile, const QString &newFile)
{
    m_old_file_name = oldFile;
    m_new_file_name = newFile;

    const QByteArray oldData = loadFromFile(oldFile);
    const QByteArray newData = loadFromFile(newFile);

    diffData(oldData, newData);
}
