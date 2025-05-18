#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>

void Buffer::buf_append_line(const std::string line){
    buffer.push_back(line);
}

void Buffer::buf_ins_char(const unsigned char c){
    if(buf_valid_row(row) && buf_valid_col(col)){
        buffer[row].insert(buffer[row].begin() + col, c);
        buf_mv_col(1);
    }
}

void Buffer::buf_new_line(void){
    if(buf_valid_row(row) && buf_valid_col(col)){
        ConstBufStrIt const_line(buffer[row]);
        const_line.offset(col).valid();
        
        const std::string substr = buf_get_substr_after_col_pos(const_line);
        if(!substr.empty()){
            BufStrIt mut_line(buffer[row]);
            mut_line.offset(col).valid();
                
            buf_erase_substr(
                mut_line
            );
        }

        buf_ins_row(row + 1, substr);
    }
}

void Buffer::buf_ins_substr(BufStrIt& str, const std::string substr){
    if(str.begin <= str.end){
        str.line.insert(str.begin, substr.begin(), substr.end());
    }
}

void Buffer::buf_ins_row(const int next, const std::string substr){
    if(buf_valid_row(row)){
        buffer.insert(buffer.begin() + next, substr);
        buf_set_col(0);
        buf_mv_row(1);
    }
}
