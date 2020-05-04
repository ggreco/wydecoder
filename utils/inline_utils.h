#ifndef UTILS_H

#define UTILS_H

#include <string>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <map>

#if defined(_MSVC)
#include <sys/timeb.h>
#include "../utils/windows_helpers.h"
#define localtime_r(x, y) localtime(x)
#else
#include <sys/time.h>
#endif

inline int64_t gettime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((int64_t)tv.tv_sec) * 1000000LL + (int64_t)tv.tv_usec;
}

inline std::string iso_date() {
    struct timeval tv;
    ::gettimeofday(&tv, NULL);
    char buf[128];
    time_t now = tv.tv_sec;
    ::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", ::gmtime(&now));
    ::sprintf(buf + ::strlen(buf), ".%03dZ", (int)(tv.tv_usec / 1000));
    return buf;
}

inline std::string double_to_string(double val) {
    char buffer[64];
    ::snprintf(buffer, sizeof(buffer), "%f", val);
    return buffer;
}

template <typename T>
inline std::string to_string(T val)
{
	std::ostringstream s;
    s << val;
    return s.str();
}

inline std::string to_lower(const std::string &orig)
{
    std::string result;
    result.reserve(orig.size());

    for (auto &a: orig)
        if (::isupper(a))
            result.push_back(::tolower(a));
        else
            result.push_back(a);

    return result;
}


inline std::string file_extension(const std::string &src)
{
    std::string::size_type p = src.find_last_of(".");
    if (p != std::string::npos && p < (src.size() - 1)) {
        return to_lower(src.substr(p + 1));
    }
    else
        return "noext";
}


inline std::string to_upper(const std::string &orig)
{
    std::string result;

    for (auto &a: orig)
        if (::isupper(a))
            result.push_back(::toupper(a));
        else
            result.push_back(a);

    return result;
}

inline std::string &trimws(std::string& tag)
{
    while (!tag.empty() &&
            ((tag[0] == ' ') || (tag[0] =='\t') ||
             (tag[0] =='\n') || (tag[0] =='\r')))
        tag.erase(0, 1);

    while (!tag.empty() && (tag.back() == ' ' || tag.back() =='\t' ||
                            tag.back() =='\n' || tag.back() =='\r'))
        tag.pop_back();

    return tag;
}


inline bool is_positive_number(std::string src) {
    if (src.empty())
        return false;
    trimws(src);
    for (auto c: src)
        if (c < '0' || c > '9')
            return false;
    return true;
}

inline void strip_characters(std::string &s, const char *c)
{
    std::string::size_type p;
    while ((p = s.find_first_of(c)) != std::string::npos)
        s.erase(p, 1);
}

inline std::string strip_spaces(const std::string &tag)
{
    std::string res;

    for (auto &a: tag)
        if (a == ' ' || a == '\t' || a == '\r' || a == '\n')
            continue;
        else
            res.push_back(a);

    return res;
}

inline std::string strip_notascii(const std::string &tag)
{
    std::string res;

    for (auto &a: tag)
        if (a <= ' ' || a > 127 || a == '\\' || a == '/' || a == ':')
            continue;
        else
            res.push_back(a);

    return res;
}

inline std::string unescape_string(const std::string &orig) {
    std::string res;
    for (size_t i = 0; i < orig.size(); ++i) {
        char a = orig[i];
        if (a == '%') {
            ++i;
            if ((i + 1) < orig.size() && ::isxdigit(orig[i]) && ::isxdigit(orig[i + 1])) {
                int c = ::strtol(orig.substr(i, 2).c_str(), nullptr, 16);
                a = c;
                ++i;
            }
        }
        res.push_back(a);
    }
    return res;
}

inline std::string only_letters(const std::string& tag)
{
    std::string res;

    for (auto &a: tag)
        if ((a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z'))
            res.push_back(a);

    return res;
}

#ifdef WIN32
inline void setenv(const char *k, const char*v, int unused) {
    std::ostringstream os;
    os << k << '=' << v;
    _putenv(os.str().c_str());
}
#endif

inline int to_int(const std::string &str)   { return atoi(str.c_str()); }
inline long to_long(const std::string &str) { return atol(str.c_str()); }
inline long long to_longlong(const std::string &str) { return atoll(str.c_str()); }
inline double to_double(const std::string &str) { return atof(str.c_str()); }

inline std::string date_now() {
    time_t now = time(nullptr);
    struct tm mytm;

    if (tm *t = ::gmtime_r(&now, &mytm)) {
        char buffer[40];
        ::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
        return buffer;
    }
    return "0000-00-00 00:00:00";
}

#endif
