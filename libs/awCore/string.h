#pragma once

#include "export.h"
#include <QString>
#include <string>

namespace aw
{

    inline std::string std_string(const char *str)
    {
        return str;
    }

    constexpr const std::string& std_string(const std::string &str)
    {
        return str;
    }

    constexpr std::string&& std_string(std::string &&str)
    {
        return std::move(str);
    }

    inline std::string std_string(const QString &str)
    {
        return str.toStdString();
    }

    inline QString qt_string(const char *str)
    {
        return str;
    }

    inline QString qt_string(const std::string &str)
    {
        return QString::fromStdString(str);
    }

    constexpr const QString& qt_string(const QString &str)
    {
        return str;
    }

    constexpr QString&& qt_string(QString &&str)
    {
        return std::move(str);
    }

    template <class string_t>
    string_t string_cast(const char *str)
    {
        return str;
    }

    template <class string_t>
    string_t string_cast(const std::string &str)
    {
        return str;
    }

    template <>
    inline QString string_cast<QString>(const std::string &str)
    {
        return qt_string(str);
    }

    template <class string_t>
    string_t string_cast(const QString &str)
    {
        return str;
    }

    template <>
    inline std::string string_cast<std::string>(const QString &str)
    {
        return std_string(str);
    }

    template <class string_t>
    struct any_string
    {
        string_t value;

        any_string() = default;
        any_string(const any_string &) = default;

        template <class string_type>
        any_string(const any_string<string_type> &str): value(string_cast<string_t>(str.value)) { }

        template <class string_type>
        any_string(const string_type &str): value(string_cast<string_t>(str)) { }

        operator string_t() const { return value; }
    };

    inline std::string substr(const std::string &str, int start, int length)
    {
        return str.substr(start, length);
    }

    inline QString substr(const QString &str, int start, int length)
    {
        return str.mid(start, length);
    }

    inline int strpos(const std::string &haystack, const std::string &needle, int offset = 0)
    {
        auto pos = haystack.find(needle, offset);
        if ( pos == haystack.npos ) return -1;
        return pos;
    }

    inline int strpos(const QString &haystack, const QString &needle, int offset = 0)
    {
        return haystack.indexOf(needle, offset, Qt::CaseSensitive);
    }

    AW_CORE_API std::string str_replace(const std::string &search, const std::string &replace, const std::string &text);

    AW_CORE_API std::string trim(const std::string &text);

    inline QString trim(const QString &text)
    {
        return text.trimmed();
    }

    inline int printf_safe_arg(short value) { return value; }
    inline unsigned printf_safe_arg(unsigned short value) { return value; }
#ifdef WIN32
    inline unsigned printf_safe_arg(long value) { return value; }
    inline unsigned long printf_safe_arg(unsigned long value) { return  value; }
#endif
    inline int printf_safe_arg(int value) { return value; }
    inline unsigned printf_safe_arg(unsigned value) { return value; }
    inline int64_t printf_safe_arg(int64_t value) { return value; }
    inline uint64_t printf_safe_arg(uint64_t value) { return value; }
    inline double printf_safe_arg(double value) { return value; }
    inline const char* printf_safe_arg(const char *s) { return s; }
    inline const char* printf_safe_arg(const std::string &s) { return s.c_str(); }
    inline const char* printf_safe_arg(const any_string<std::string> &s) { return s.value.c_str(); }
    constexpr const void * printf_safe_arg(const void *p) { return p; }

    inline std::string std_printf(const char *fmt)
    {
        return fmt;
    }

    inline const std::string std_printf(const any_string<std::string> &message)
    {
        return message.value;
    }

    inline QString qt_printf(const char *fmt)
    {
        return fmt;
    }

    AW_CORE_API std::string asprintf_std(const char *fmt, ...);

    template <typename ...Args>
    inline std::string std_printf(const char *fmt, const Args&... args)
    {
        return asprintf_std(fmt, printf_safe_arg(args)...);
    }

    AW_CORE_API QString asprintf_qt(const char *fmt, ...);

    template <typename ...Args>
    inline QString qt_printf(const char *fmt, const Args&... args)
    {
        return asprintf_qt(fmt, printf_safe_arg(args)...);
    }

}
