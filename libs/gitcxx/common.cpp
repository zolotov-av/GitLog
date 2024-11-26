#include "common.h"
#include "exception.h"
#include <awCore/trace.h>

namespace git
{

    QString lastGitError()
    {
        const auto err = git_error_last();
        if ( err == nullptr )
            return "no error";

        return QString::fromUtf8(err->message);
    }

    void check(int error)
    {
        if (error < 0)
        {
            throw exception("%s", lastGitError());
        }
    }

    void warn(int error)
    {
        if (error < 0)
        {
            aw::trace::log("warn: %s", lastGitError());
        }
    }

}
