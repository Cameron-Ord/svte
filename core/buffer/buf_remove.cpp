#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"


#include <iostream>

void Buffer::buf_zero_buffer(void)
{
    buffer.clear();
}

void Buffer::buf_zero_line(std::string &str)
{
    str.clear();
}

void Buffer::buf_erase_substr(BufStrIt &str)
{
    if (str.begin != str.end) {
        str.line.erase(str.begin, str.end);
    }
}

void Buffer::buf_erase_char(BufStrIt &str)
{
    if (str.begin != str.end) {
        str.line.erase(str.begin);
    }
}

void Buffer::buf_rmv_line(BufRowIt &vec)
{
    if (vec.begin != vec.end) {
        buffer.erase(vec.begin);
    }
}

void Buffer::buf_rmv_before(void)
{
    if (buf_valid_row(row) && buf_valid_col(col)) {
        if (col > 0) {

            BufStrIt mut_line(buffer[row]);
            mut_line.offset(col - 1).valid();

            buf_erase_char(mut_line);
            buf_mv_col(-1);

        } else if (col == 0 && row > 0) {
            ConstBufStrIt const_line(buffer[row]);

            std::string substr = buf_get_substr_after_col_pos(const_line);
            BufRowIt mut_lines(buffer);
            mut_lines.offset(row).valid();

            buf_rmv_line(mut_lines);
            buf_mv_row(-1);

            BufStrIt concat_line(buffer[row]);
            const size_t prev_size = buffer[row].size();
            concat_line.offset(prev_size).valid();

            buf_ins_substr(concat_line, substr);

            buf_mv_col(prev_size);
        }
    }
}

void Buffer::buf_rmv_at(void)
{
    if (buf_valid_row(row) && buf_valid_col(col)) {
        if (col >= 0 && buf_get_line_size(row) > 0) {
            if (col > 0 && col == buf_get_line_size(row)) {
                buf_mv_col(-1);
            }

            BufStrIt mut_line(buffer[row]);
            mut_line.offset(col).valid();
            buf_erase_char(mut_line);

        } else if (col == 0 && row > 0 && buf_get_line_size(row) == 0) {
            const int tmp = row;

            if (row == buf_get_size() - 1) {
                buf_mv_row(-1);
            }

            BufRowIt mut_lines(buffer);
            mut_lines.offset(tmp).valid();
            buf_rmv_line(mut_lines);
        }
    }
}
