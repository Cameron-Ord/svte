#ifndef CORE_STRUCTS_HPP
#define CORE_STRUCTS_HPP
class Buffer;

struct BCommit {
    int32_t id;
    const class Buffer *cbuf;
    BCommit(const int32_t ret_id, const class Buffer *buffer) : id(ret_id), cbuf(buffer) {}

};
typedef BCommit BCommit;

#endif