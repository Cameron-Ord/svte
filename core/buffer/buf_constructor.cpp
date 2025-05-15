
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"
#include "../../include/core/core_defines.hpp"

//Buffer constructor + getters/setters

#include <iostream>

Buffer::Buffer(const int32_t id) : error(CORE_NIL) {
    buf_set_id(id);
    buf_zero_buffer();
    row = 0, col = 0;

    std::cout << "Buffer created with ID: " << id << std::endl;
}

Buffer::Buffer(const int32_t id, std::string fn) : error(CORE_NIL) {
    buf_set_id(id);
    buf_set_filename(fn);
    buf_zero_buffer();
    row = 0, col = 0;

    std::cout << "Buffer created with ID: " << id << " ";
    std::cout << "- Filename: " << fn << std::endl;
}

Buffer::~Buffer(void){

}

std::string::const_iterator Buffer::buf_str_it_begin(const std::string& str) const {
    return str.begin();
}
std::string::const_iterator Buffer::buf_str_it_end(const std::string& str) const {
    return str.end();
}

const int Buffer::buf_get_line_size(const int access) const {
    if(buf_valid_row(access)){
        return static_cast<int>(buffer[access].size());
    }
    return -1;
}

const int Buffer::buf_get_size(void) const {
    return static_cast<int>(buffer.size());
}

std::vector<std::string>::const_iterator Buffer::buf_row_begin_const(void) const {
    return buffer.begin();
}

std::vector<std::string>::const_iterator Buffer::buf_row_end_const(void) const {
    return buffer.end();
}

std::vector<std::string>::iterator Buffer::buf_row_begin(void){
    return buffer.begin();
}

std::vector<std::string>::iterator Buffer::buf_row_end(void){
    return buffer.end();
}

const std::vector<std::string>* const Buffer::buf_get_buffer(void) const {
    return &buffer;
}

void Buffer::buf_set_id(const int32_t id){
    buffer_id = id;
}

const int32_t& Buffer::buf_get_id(void) const {
    return buffer_id;
}

void Buffer::buf_set_filename(const std::string fn){
    filename = fn;
}

const std::string& Buffer::buf_get_filename(void) const {
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

const int Buffer::buf_get_row(void) const {
    return row;
}

const int Buffer::buf_get_col(void) const {
    return col;
}