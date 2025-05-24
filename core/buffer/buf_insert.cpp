#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_token.hpp"

#include <iostream>

void Buffer::buf_ins_char(const char c)
{
    if (buf_valid_row(row) && buf_valid_col(col)) {
        Token t = { c, UNKNOWN_TOKEN };
        buffer[row].insert(buffer[row].begin() + col, t);
    }
}

void Buffer::buf_new_line(void)
{
    if (buf_valid_row(row) && buf_valid_col(col)) {
        std::vector<Token>& tokenline = buffer[row];
        ColItConst token_it_const(tokenline);
        token_it_const.offset(col);
        const std::vector<Token> tokens = buf_tokens_after_col(token_it_const.begin, token_it_const.end);
        if (!tokens.empty()) {
            ColIt token_it(tokenline);
            token_it.reset(tokenline);
            token_it.offset(col);
            buf_erase_tokens(token_it, tokenline);
        }

        buf_ins_row(row + 1, tokens);
        buf_set_col(0);
        buf_mv_row(1);
    }
}

void Buffer::buf_ins_tokens(ColIt& tokens, std::vector<Token>& line, const std::vector<Token> sub)
{
    if (tokens.begin <= tokens.end) {
        line.insert(tokens.begin, sub.begin(), sub.end());
    }
}

void Buffer::buf_ins_row(const int next, const std::vector<Token> sub)
{
    if (buf_valid_row(row)) {
        buffer.insert(buffer.begin() + next, sub);
    }
}
