
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"
#include "../../include/core/core_defines.hpp"


const int Buffer::buf_valid_row(const int srow){
    const int ssize = buf_get_size();
    if(srow >= 0 && srow < ssize){
        return 1;
    }
    return 0;
}

const int Buffer::buf_valid_col(const int scol){
    const int ssize = buf_get_line_size(row);
    if(ssize >= 0 && scol >= 0 && scol <= ssize){
        return 1;
    }
    return 0;
}

void Buffer::buf_mv_col(const int amount){
    if(buf_valid_col(col + amount)){
        col += amount;
    }
}


int Buffer::buf_mv_row(const int amount){
    if(buf_valid_row(row + amount)){
        row += amount;
    }
    return row;
}

void Buffer::buf_update_col(const int updated_row){
    if(buf_valid_row(updated_row)){
        const int ssize = buf_get_line_size(row);
        if(col > ssize){
            col = ssize;
            return;
        }

        const int i = buf_index_of_first_char(buf_str_it_begin(buffer[row]), buf_str_it_end(buffer[row]));
        if(i >= 0 && col < i){
            col = i;
            return;
        }
    }
}