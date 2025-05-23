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
            ColIt token_it(buffer[row]);
            token_it.offset(col - 1);
            buf_erase_token(token_it, buffer[row]);
            buf_mv_col(-1);
        } else if (col == 0 && row > 0) {
            ColItConst token_line_it(buffer[row]);
            std::vector<Token> tokens = buf_tokens_after_col(token_line_it.begin, token_line_it.end);
            
            RowIt line(buffer);
            line.offset(row);
            buf_rmv_line(line);
            buf_mv_row(-1);

            ColIt concat_line_it(buffer[row]);
            const size_t prev_size = buffer[row].size();

            concat_line_it.offset(prev_size);
            buf_ins_tokens(concat_line_it, buffer[row], tokens);
            buf_mv_col(prev_size);
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

        ColIt line(buffer[row]);
        line.offset(col);
        buf_erase_token(line, buffer[row]);

    } else if (col == 0 && row > 0 && buf_get_line_size(row) == 0) {
        const int tmp = row;

        if (row == buf_get_size() - 1) {
            buf_mv_row(-1);
        }

        RowIt rows(buffer);
        rows.offset(tmp);
        buf_rmv_line(rows);
    }
}
