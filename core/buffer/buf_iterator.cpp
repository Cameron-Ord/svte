
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_token.hpp"

std::vector<Token> Buffer::buf_tokens_after_col(ColIt &str)
{
    std::vector<Token> tokens;
    for (; str.begin != str.end; str.begin++) {
        tokens.push_back({str.begin->token, str.begin->identifier});
    }
    return tokens;
}