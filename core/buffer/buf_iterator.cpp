
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"
#include "../../include/core/core_defines.hpp"


const int Buffer::buf_index_of_first_char(std::string::const_iterator it, std::string::const_iterator end){
    for(int i = 0; it != end; ++it, ++i){
        const unsigned char c = *it;
        if(c != SPACECHAR){
            return i;
        }
    }
    return -1;
}

std::string Buffer::buf_get_substr_after_col_pos(ConstRangeStr& str){
    std::string substr;
    for(; str.begin != str.end; str.begin++){
        const unsigned char c = *str.begin;
        substr += c;
    }
    return substr;
}

std::string Buffer::buf_get_substr_before_col_pos(ConstRangeStr& str){
    std::string substr;
    for(; str.begin != str.end; str.begin++){
        const unsigned char c = *str.begin;
        substr += c; 
    }
    return substr;
}