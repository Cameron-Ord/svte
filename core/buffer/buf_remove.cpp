#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_token.hpp"



#include <iostream>

void Buffer::buf_zero_buffer(void)
{
    buffer.clear();
}

void Buffer::buf_zero_line(std::string &str)
{
    str.clear();
}

void Buffer::buf_erase_tokens(ColIt &tokens, std::vector<Token>& line)
{
    if (tokens.begin != tokens.end) {
        line.erase(tokens.begin, tokens.end);
    }
}

void Buffer::buf_erase_token(ColIt &tokens, std::vector<Token>& line)
{
    if (tokens.begin != tokens.end) {
        line.erase(tokens.begin);
    }
}

void Buffer::buf_rmv_line(RowIt &token_rows)
{
    if (token_rows.begin != token_rows.end) {
        buffer.erase(token_rows.begin);
    }
}

void Buffer::buf_rmv_before(void)
{
    if (buf_valid_row(row) && buf_valid_col(col)) {
        if (col > 0) {
            std::vector<Token>& tokenline = buffer[row];
            ColIt token_it(tokenline);
            token_it.offset(col - 1);
            buf_erase_token(token_it, tokenline);
        } else if (col == 0 && row > 0) {
            

        }
    }
}

void Buffer::buf_rmv_at(void)
{
    if (!buf_valid_row(row) || !buf_valid_col(col)) {
        return;
    }

    if (col >= 0 && buf_get_line_size(row) > 0) {
        if (col > 0 && col == buf_get_line_size(row)) {
            buf_mv_col(-1);
        }


    } else if (col == 0 && row > 0 && buf_get_line_size(row) == 0) {
        const int tmp = row;

        if (row == buf_get_size() - 1) {
            buf_mv_row(-1);
        }

    }
}
