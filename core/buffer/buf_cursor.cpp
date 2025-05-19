
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>

const int Buffer::buf_valid_row(const int srow) const
{
    const int ssize = buf_get_size();
    if (srow >= 0 && srow < ssize) {
        return 1;
    }
    return 0;
}

const int Buffer::buf_valid_col(const int scol) const
{
    const int ssize = buf_get_line_size(row);
    if (ssize >= 0 && scol >= 0 && scol <= ssize) {
        return 1;
    }
    return 0;
}

void Buffer::buf_mv_col(const int amount)
{
    if (buf_valid_col(col + amount)) {
        col += amount;
    }
    saved_col = col;
}

int Buffer::buf_mv_row(const int amount)
{
    if (buf_valid_row(row + amount)) {
        row += amount;
    }
    return row;
}

void Buffer::buf_update_col(const int updated_row)
{
    if (buf_valid_row(updated_row)) {
        const int ssize = buf_get_line_size(row);
        if (col > ssize) {
            col = ssize;
            return;
        }
        col = (saved_col <= ssize) ? saved_col : ssize;
    }
}