#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

void Buffer::buf_zero_buffer(void){
    buffer.clear();
}

void Buffer::buf_zero_line(std::string& str){
    str.clear();
}