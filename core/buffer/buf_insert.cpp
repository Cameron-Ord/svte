#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>

void Buffer::buf_append_line(const std::string line){
    buffer.push_back(line);
}

void Buffer::buf_ins_char(const unsigned char c){
    std::vector<std::string>::iterator it = buf_get_it_at_row(buffer.begin(), buffer.end());
    if(it != buffer.end() && col <= (int)it->size() && col >= 0){
        it->insert(it->begin() + col, c);
        col++;
    }
}