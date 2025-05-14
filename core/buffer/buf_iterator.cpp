
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