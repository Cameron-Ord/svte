#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_defines.hpp"


#include <iostream>
#include <random>

const std::vector<int32_t> &Editor::ed_get_open(void)
{
    return open;
}

int32_t Editor::ed_next_id(void){
    for(size_t i = 0; i < open.size(); i++){
        const size_t next = i + 1;
        if(open[i] == current_buffer_id && next < open.size()){
            return open[next];
        } else if (open[i] == current_buffer_id && next >= open.size()){
            return open[0];
        }
    }
    return -1;
}

int32_t Editor::ed_prev_id(void){
    const int ssize = static_cast<int>(open.size());
    for(int i = ssize - 1; i >= 0; --i){
        const int prev = i - 1;
        if(open[i] == current_buffer_id && prev >= 0){
            return open[prev];
        } else if (open[i] == current_buffer_id && prev < 0){
            return open[ssize - 1];
        }
    }
    return -1;
}

int32_t Editor::ed_gen_id(void)
{
    std::random_device d;
    //mersenne twister engine - check this out later.
    std::mt19937 rng(d());
    std::uniform_int_distribution<int32_t> dist(0, INT32_MAX);

    int attempt = 0;
    const int max_attempts = 1000;
    std::unordered_set<int32_t>::iterator it;

    while (attempt < max_attempts) {
        //random n between 0 and int32 max
        const int32_t generated = dist(rng);
        it = used_ids.find(generated);
        if (it == used_ids.end()) {
            return generated;
        }
        attempt++;
    }
    //return a negative id
    return BAD_ID;
}

int Editor::ed_append_buffer(std::pair<std::string, std::vector<std::string>> commit)
{
    const int32_t id = ed_gen_id();
    if (id == BAD_ID) {
        std::cerr << "Could not create a valid ID" << std::endl;
        return ED_BAD_APPEND;
    }

    class Buffer *buffer = new Buffer(id);
    if (!buffer) {
        std::cerr << "Could not allocate buffer!" << std::endl;
        return ED_BAD_APPEND;
    }
    buffer->buf_copy(commit.second).buf_set_filename(commit.first);
    
    bufs.insert({id, buffer});
    open.push_back(id);
    used_ids.insert(id);

    ed_set_current_id(id);
    return id;
}


const Buffer *const Editor::ed_fetch_buffer_const(const int32_t id) const
{
    std::unordered_map<int32_t, class Buffer *>::const_iterator it = bufs.find(id);
    if (it != bufs.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

Buffer *Editor::ed_fetch_buffer(const int32_t id)
{
    std::unordered_map<int32_t, class Buffer *>::iterator it = bufs.find(id);
    if (it != bufs.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}