
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_parser.hpp"


#include <iostream>

Buffer::Buffer(const int32_t id) : error(CORE_NIL)
{
    buf_set_id(id);
    buf_zero_buffer();
    row = 0, col = 0, saved_col = 0;

    std::cout << "Buffer created with ID: " << id << std::endl;
}

Buffer::Buffer(const int32_t id, std::string fn) : error(CORE_NIL)
{
    buf_set_id(id);
    buf_set_filename(fn);
    buf_zero_buffer();
    row = 0, col = 0, saved_col = 0;

    std::cout << "Buffer created with ID: " << id << " ";
    std::cout << "- Filename: " << fn << std::endl;
}

std::vector<std::vector<Token>> Buffer::buf_tokenize(const std::vector<std::string> readbuffer){
    TokenParser tparser(readbuffer);
    return tparser.tokenize();
}

void Buffer::buf_retokenize(void){
}

Buffer::~Buffer(void)
{
}