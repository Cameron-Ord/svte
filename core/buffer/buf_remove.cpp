#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>

void Buffer::buf_zero_buffer(void){
    buffer.clear();
}

void Buffer::buf_zero_line(std::string& str){
    str.clear();
}

void Buffer::buf_erase_substr(MutRangeStr& str){
    if(str.begin != str.end && str.str){
        str.str->erase(str.begin, str.end);
    }
}

void Buffer::buf_erase_char(MutRangeStr& str){
    if(str.begin != str.end && str.str){
        str.str->erase(str.begin);
    }
}

void Buffer::buf_rmv_line(MutRangeVecStr& vec){
    if(vec.begin != vec.end){
        buffer.erase(vec.begin);
    }
}

void Buffer::buf_rmv_before(void){
    if(buf_valid_row(row) && buf_valid_col(col)){
        if(col > 0){

            MutRangeStr mut_line(
                buf_str_it_end(buffer[row]), 
                buf_str_it_begin(buffer[row]) + (col - 1)
            );
            
            buf_erase_char(
                mut_line.use_str(&buffer[row])
            );
            buf_mv_col(-1);

        } else if(col == 0 && row > 0){
            ConstRangeStr const_line(
                buf_str_it_end_const(buffer[row]),
                buf_str_it_begin_const(buffer[row])
            );  

            std::string substr = buf_get_substr_after_col_pos(const_line);
            MutRangeVecStr mut_lines(buf_row_end(), buf_row_begin() + row);

            buf_rmv_line(mut_lines);
            buf_mv_row(-1);

            MutRangeStr concat_line(
                buf_str_it_end(buffer[row]), 
                buf_str_it_end(buffer[row])
            );
            const size_t prev_size = buffer[row].size();

            buf_ins_substr(
                concat_line.use_str(&buffer[row]),
                substr
            );

            buf_mv_col(prev_size);
        }
    }
}

void Buffer::buf_rmv_at(void){
    if(buf_valid_row(row) && buf_valid_col(col)){
        if(col >= 0 && buf_get_line_size(row) > 0){
            if(col > 0 && col == buf_get_line_size(row)){
                buf_mv_col(-1);
            }

            MutRangeStr mut_line(
                buf_str_it_end(buffer[row]), 
                buf_str_it_begin(buffer[row]) + col
            );
            
            buf_erase_char(
                mut_line.use_str(&buffer[row])
            );


        } else if(col == 0 && row > 0 && buffer[row].size() == 0){
            ConstRangeStr const_line(
                buf_str_it_end_const(buffer[row]),
                buf_str_it_begin_const(buffer[row])
            );  

            std::string substr = buf_get_substr_after_col_pos(const_line);
            MutRangeVecStr mut_lines(buf_row_end(), buf_row_begin() + row);

            buf_rmv_line(mut_lines);
            buf_mv_row(-1);

            MutRangeStr concat_line(
                buf_str_it_end(buffer[row]), 
                buf_str_it_end(buffer[row])
            );
            const int prev_size = buf_get_line_size(row);

            buf_ins_substr(
                concat_line.use_str(&buffer[row]),
                substr
            );

            buf_mv_col(prev_size);
        }
    }
}
