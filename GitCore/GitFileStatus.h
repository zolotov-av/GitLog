#pragma once

#include <QString>
#include <QColor>
#include <gitcxx/diff_delta.h>

/**
 * @brief Информация о статусе файла для GitStatusModel
 * @ingroup GitCore
 */
class GitFileStatus
{
public:

    enum Source {
        Untracked,
        Worktree,
        Stage,
        Conflict
    };

private:

    Source m_status_source { Untracked };
    QString m_file_name;
    git_delta_t m_file_status;

public:

    GitFileStatus() = default;
    GitFileStatus(Source src, const git::diff_delta &delta);
    GitFileStatus(const GitFileStatus &) = default;
    GitFileStatus(GitFileStatus &&) = default;
    ~GitFileStatus() = default;

    GitFileStatus& operator = (const GitFileStatus &) = default;
    GitFileStatus& operator = (GitFileStatus &&) = default;

    static const char* sourceName(Source src);
    static QColor sourceColor(Source src);
    static QString getFilePath(const git::diff_delta &delta);
    static const char* getFileStatus(git_delta_t status);
    static const char* getFileStatus(const git::diff_delta &delta);

    const char* statusSource() const { return sourceName(m_status_source); }
    const QString& fileName() const { return m_file_name; }
    git_delta_t status() const { return m_file_status; }
    const char* statusName() const { return getFileStatus(m_file_status); }
    const QColor fileColor() const { return sourceColor(m_status_source); }

};
