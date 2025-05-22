#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_parser.hpp"


void Buffer::buf_tokenize(void){
    TokenParser tparser(buffer);
    group_buffer = tparser.tokenize_by_group();
}

