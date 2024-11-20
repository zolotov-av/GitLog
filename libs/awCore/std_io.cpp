#include "std_io.h"

namespace aw
{

    AW_CORE_API void atomic_put_line(FILE *stream, const char *message)
    {
        char *text;
        const auto ret = asprintf(&text, "%s\n", message);
        if ( ret < 0 ) // error
            return;

        fputs(text, stream);
        free(text);
    }

}
