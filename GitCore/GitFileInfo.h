#pragma once

#include <QString>
#include <gitcxx/diff_delta.h>

class GitFileInfo
{
private:

    QString m_file_name;
    git_delta_t m_file_status;

public:

    GitFileInfo() = default;
    GitFileInfo(const git::diff_delta &delta);
    GitFileInfo(const GitFileInfo &) = default;
    GitFileInfo(GitFileInfo &&) = default;
    ~GitFileInfo() = default;

    GitFileInfo& operator = (const GitFileInfo &) = default;
    GitFileInfo& operator = (GitFileInfo &&) = default;

    static QString getFilePath(const git::diff_delta &delta);
    static const char* getFileStatus(git_delta_t status);
    static const char* getFileStatus(const git::diff_delta &delta);

    const QString& fileName() const { return m_file_name; }
    git_delta_t status() const { return m_file_status; }
    const char* statusName() const { return getFileStatus(m_file_status); }

};
