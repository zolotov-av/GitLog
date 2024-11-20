#include "log.h"
#include "std_io.h"
#include "thread.h"
#include <QDateTime>
#include <QThreadStorage>
#include <atomic>

namespace aw
{

    struct LoggerState
    {
        std::atomic<FILE*> stream { nullptr };
        QThreadStorage<long> indent { };
    };

    static LoggerState logger { };

    void pushLogIndent()
    {
        logger.indent.setLocalData(logger.indent.localData() + 1);
    }

    void popLogIndent()
    {
        logger.indent.setLocalData(logger.indent.localData() - 1);
    }

    void log(const char* message)
    {
        auto stream = logger.stream.load(std::memory_order_relaxed);
        if ( stream )
        {
            const auto time = QDateTime::currentDateTime().toString("dd MMM yyyy HH:mm:ss.zzz");;
            const auto thread = currentThreadName();
            const std::string indent(logger.indent.localData(), '.');
            atomic_put_line(stream, "%s   %10s  %s%s", time, thread, indent, message);
        }
    }

    void setLogStream(FILE* s)
    {
        logger.stream.store(s, std::memory_order_release);
    }

}
