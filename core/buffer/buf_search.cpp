
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_token.hpp"

std::vector<Token> Buffer::buf_tokens_after_col(std::vector<Token>::const_iterator& begin, std::vector<Token>::const_iterator& end)
{
    std::vector<Token> tokens;
    for (; begin != end; begin++) {
        tokens.push_back({begin->token, begin->identifier});
    }
    return tokens;
}
