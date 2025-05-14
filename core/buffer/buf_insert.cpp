#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>

void Buffer::buf_append_line(const std::string line){
    buffer.push_back(line);
}

void Buffer::buf_ins_char(const unsigned char c){
    if(buf_valid_row(row) && buf_valid_col(col)){
        buffer[row].insert(buffer[row].begin() + col, c);
        col++;
    }
}