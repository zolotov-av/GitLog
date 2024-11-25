#include "GitFileInfo.h"

GitFileInfo::GitFileInfo(const git::diff_delta &delta):
    m_file_name{getFilePath(delta)}, m_file_status{delta.type()}
{

}

QString GitFileInfo::getFilePath(const git::diff_delta &delta)
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
    default:
        return "N/A";
    }
}

const char *GitFileInfo::getFileStatus(git_delta_t status)
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
    default:
        return "unknown";
    }
}

const char* GitFileInfo::getFileStatus(const git::diff_delta &delta)
{
    return getFileStatus(delta.type());
}
