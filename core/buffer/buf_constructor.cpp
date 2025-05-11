
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

Buffer::Buffer(const int32_t id) : error(CORE_NIL) {
    buf_set_id(id);
    buf_zero_buffer();
}

Buffer::Buffer(const int32_t id, std::string fn) {
    buf_set_id(id);
    buf_set_filename(fn);
    buf_zero_buffer();
}

Buffer::~Buffer(void){

}

void Buffer::buf_set_id(const int32_t id){
    buffer_id = id;
}

const int32_t& Buffer::buf_get_id(void){
    return buffer_id;
}

void Buffer::buf_set_filename(const std::string fn){
    filename = fn;
}

const std::string& Buffer::buf_get_filename(void){
    return filename;
}

void Buffer::buf_set_error(const int err){
    error = err;
}

const int& Buffer::buf_get_error(void){
    return error;
}

void Buffer::buf_set_row(const int val){
    row = val;
}

void Buffer::buf_set_col(const int val){
    col = val;
}

const int& Buffer::buf_get_row(void){
    return row;
}

const int& Buffer::buf_get_col(void){
    return col;
}