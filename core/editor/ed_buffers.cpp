#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>
#include <random>


const std::vector<int32_t>& Editor::ed_get_open(void){
    return open;
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
            used_ids.insert(generated);
            return generated;
        }
        attempt++;
    }
    //return a negative id
    return BAD_ID;
}

int Editor::ed_append_buffer(void){
    const int32_t id = ed_gen_id();
    if(id == BAD_ID){
        std::cerr << "Could not create a valid ID" << std::endl;
        return ED_BAD_APPEND;
    }

    std::cout << "Appending: " << id << std::endl;
    class Buffer *buffer = new Buffer(id);
    if(!buffer){
        std::cerr << "Could not allocate buffer!" << std::endl;
        used_ids.erase(id);
        return ED_BAD_APPEND;
    }

    bufs.insert({id, buffer});
    open.push_back(id);
    ed_set_current_id(id);
    return id;
}

int Editor::ed_append_buffer(std::string fn){
    const int32_t id = ed_gen_id();
    if(id == BAD_ID){
        std::cerr << "Could not create a valid ID" << std::endl;
        return ED_BAD_APPEND;
    }
    std::cout << "Appending: " << id << std::endl;
    class Buffer *buffer = new Buffer(id, fn);
    if(!buffer){
        std::cerr << "Could not allocate buffer!" << std::endl;
        used_ids.erase(id);
        return ED_BAD_APPEND;
    }

    bufs.insert({id, buffer});
    open.push_back(id);
    ed_set_current_id(id);
    
    return id;
}

const class Buffer *Editor::ed_fetch_buffer_const(const int32_t id){
    std::unordered_map<int32_t, class Buffer *>::const_iterator it = bufs.find(id);
    if(it != bufs.end()){
        return it->second;
    } else {
        return nullptr;
    }
}

class Buffer *Editor::ed_fetch_buffer(const int32_t id){
    std::unordered_map<int32_t, class Buffer *>::iterator it = bufs.find(id);
    if(it != bufs.end()){
        return it->second;
    } else {
        return nullptr;
    }
}

void Editor::ed_ins_char(const int id, const unsigned char c){
    class Buffer *b = ed_fetch_buffer(id);
    if(!b){
        return;
    }

    switch(mode){
        default:break;
        case APPEND:{

        }break;

        case INSERT:{
            b->buf_ins_char(c);
        }break;
    }
}


void Editor::ed_mv_cursor_row(const int id, const int amount){
    class Buffer *b = ed_fetch_buffer(id);
    if(!b){
        return;
    }
   b->buf_update_col(b->buf_mv_row(amount));
}

void Editor::ed_mv_cursor_col(const int id, const int amount){
    class Buffer *b = ed_fetch_buffer(id);
    if(!b){
        return;
    }
    b->buf_mv_col(amount);
}

const int Editor::ed_get_col(const int id){
    class Buffer *b = ed_fetch_buffer(id);
    if(!b){
        return -1;
    }
    return b->buf_get_col();
}

const int Editor::ed_get_row(const int id){
    class Buffer *b = ed_fetch_buffer(id);
    if(!b){
        return -1;
    }  
    return b->buf_get_row();
}
