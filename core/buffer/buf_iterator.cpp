
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_defines.hpp"


std::string Buffer::buf_get_substr_after_col_pos(ConstBufStrIt &str)
{
    std::string substr;
    for (; str.begin != str.end; str.begin++) {
        const unsigned char c = *str.begin;
        substr += c;
    }
    return substr;
}

std::string Buffer::buf_get_substr_before_col_pos(ConstBufStrIt &str)
{
    //probably use a reverse it and --it
}