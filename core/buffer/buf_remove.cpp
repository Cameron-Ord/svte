#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

void Buffer::buf_zero_buffer(void){
    buffer.clear();
}

void Buffer::buf_zero_line(std::string& str){
    str.clear();
}

void Buffer::buf_erase_substr(std::string& line, MutRangeStr str){
    line.erase(str.begin, str.end);
}

void Buffer::buf_erase_char(std::string& line, MutRangeStr str){
    line.erase(str.begin);
}

void Buffer::buf_rmv_before(void){
    if(buf_valid_row(row) && buf_valid_col(col)){
        if(col > 0){
            buf_erase_char(
                buffer[row],
                MutRangeStr(buf_str_it_end(buffer[row]), buf_str_it_begin(buffer[row]) + (col - 1))
            );
            buf_mv_col(-1);
        }
    }
}

void Buffer::buf_rmv_at(void){
    if(buf_valid_row(row) && buf_valid_col(col)){
        
    }
}
