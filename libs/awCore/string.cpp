#include "string.h"
#include <stdarg.h>

namespace aw
{

    std::string str_replace(const std::string &search, const std::string &replace, const std::string &text)
    {
        const int slen = search.length();
        if ( slen == 0 ) return text;

        int offset = 0;
        std::string result;
        const int text_len = text.length();
        while ( offset < text_len )
        {
            auto pos = text.find(search, offset);
            if ( pos == text.npos ) return result + text.substr(offset);
            result += text.substr(offset, pos - offset) + replace;
            offset = pos + slen;
        }
        return result;
    }

    std::string trim(const std::string &text)
    {
        if ( text.length() == 0 ) return {};

        const char *begin = text.c_str();
        const char *end = text.c_str() + text.length() - 1;
        while ( std::isspace(*begin) ) begin++;
        while ( end > begin && std::isspace(*end) ) end--;

        return std::string(begin, end - begin + 1);
    }

    std::string asprintf_std(const char *fmt, ...)
    {
        va_list va;
        va_start(va, fmt);

        char *buf;
        const int ret = ::vasprintf(&buf, fmt, va);

        va_end(va);

        if ( ret > 0 ) {
            std::string text(buf, ret);
            ::free(buf);
            return text;
        }

        return "vasprintf() error";
    }

    QString asprintf_qt(const char *fmt, ...)
    {
        va_list va;
        va_start(va, fmt);

        char *buf;
        const int ret = ::vasprintf(&buf, fmt, va);

        va_end(va);

        if ( ret > 0 ) {
            QString text = QString::fromUtf8(buf, ret);
            ::free(buf);
            return text;
        }

        return "vasprintf() error";
    }

}
