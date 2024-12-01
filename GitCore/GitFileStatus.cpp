#include "GitFileStatus.h"

GitFileStatus::GitFileStatus(Source src, const git::diff_delta &delta):
    m_status_source{src}, m_file_name{getFilePath(delta)}, m_file_status{delta.type()}
{

}

const char* GitFileStatus::sourceName(Source src)
{
    switch(src)
    {
    case Untracked: return "untracked";
    case Worktree: return "worktree";
    case Stage: return "stage";
    case Conflict: return "conflict";
    default: return "unknown";
    }
}

QColor GitFileStatus::sourceColor(Source src)
{
    constexpr QColor cachedColor{0x18, 0xb2, 0x18};
    constexpr QColor modifiedColor{0xb2, 0x18, 0x18};
    constexpr QColor untrackedColor{0x35, 0x35, 0x35};
    constexpr QColor conflictedColor{0xFF, 0x98, 0x00};

    switch(src)
    {
    case Untracked: return untrackedColor;
    case Worktree: return modifiedColor;
    case Stage: return cachedColor;
    case Conflict: return conflictedColor;
    default: return Qt::black;
    }
}

QString GitFileStatus::getFilePath(const git::diff_delta &delta)
{
    switch ( delta.type() )
    {
    case GIT_DELTA_IGNORED:
    case GIT_DELTA_UNTRACKED:
    case GIT_DELTA_TYPECHANGE:
    case GIT_DELTA_ADDED:
    case GIT_DELTA_COPIED:
        return delta.newFile().path();
    case GIT_DELTA_UNMODIFIED:
    case GIT_DELTA_DELETED:
    case GIT_DELTA_MODIFIED:
    case GIT_DELTA_RENAMED:
        return delta.oldFile().path();
    case GIT_DELTA_CONFLICTED:
    case GIT_DELTA_UNREADABLE:
        if ( !delta.oldFile().path().isEmpty() )
            return delta.oldFile().path();
        if ( !delta.newFile().path().isEmpty() )
            return delta.newFile().path();
    default:
        return "N/A";
    }
}

const char *GitFileStatus::getFileStatus(git_delta_t status)
{
    switch ( status )
    {
    case GIT_DELTA_UNMODIFIED:
        return "unmodified";
    case GIT_DELTA_ADDED:
        return "new";
    case GIT_DELTA_DELETED:
        return "deleted";
    case GIT_DELTA_MODIFIED:
    case GIT_DELTA_RENAMED:
    case GIT_DELTA_COPIED:
        return "modified";
    case GIT_DELTA_IGNORED:
        return "ignored";
    case GIT_DELTA_UNTRACKED:
        return "untracked";
    case GIT_DELTA_TYPECHANGE:
        return "typechange";
    case GIT_DELTA_CONFLICTED:
        return "conflicted";
    case GIT_DELTA_UNREADABLE:
        return "unreadable";
    default:
        return "unknown";
    }
}

const char* GitFileStatus::getFileStatus(const git::diff_delta &delta)
{
    return getFileStatus(delta.type());
}
