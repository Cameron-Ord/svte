#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

void Buffer::buf_append_line(const std::string line){
    buffer.push_back(line);
}