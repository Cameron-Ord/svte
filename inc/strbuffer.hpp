#ifndef STRBUFFER_HPP
#define STRBUFFER_HPP
#include <cstddef>

#define STRLIMIT 256

struct String
{
    char str[STRLIMIT];
    size_t len;
};

#endif
