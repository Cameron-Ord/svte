#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>

int32_t Editor::ed_gen_id(void)
{
    int attempt = 0;
    const int max_attempts = 1000;
    std::unordered_set<int32_t>::iterator it;

    while (attempt < max_attempts) {
        //random n between 0 and int32 max
        const int32_t generated = rand() % INT32_MAX;
        it = used_ids.find(generated);
        if (it == used_ids.end()) {
            used_ids.insert(generated);
            return generated;
        }
        attempt++;
    }
    //return a negative id
    return CORE_NIL;
}

int Editor::ed_append_buffer(void){
    const int32_t id = ed_gen_id();
    if(id == CORE_NIL){
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

    std::pair<int32_t, class Buffer*> mapped = { id, buffer };
    bufs.insert(mapped);
    open.push_back(id);
    ed_set_current_id(id);
    
    return CORE_NIL;
}

int Editor::ed_append_buffer(std::string fn){
    const int32_t id = ed_gen_id();
    if(id == CORE_NIL){
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

    std::pair<int32_t, class Buffer*> mapped = { id, buffer };
    bufs.insert(mapped);
    open.push_back(id);
    ed_set_current_id(id);
    
    return CORE_NIL;
}

class Buffer *Editor::ed_fetch_buffer(const int32_t id){
    std::unordered_map<int32_t, class Buffer *>::iterator it = bufs.find(id);
    if(it != bufs.end()){
        return it->second;
    } else {
        return nullptr;
    }
}

void Editor::ed_ins_char(const unsigned char c){
    class Buffer *b = ed_fetch_buffer(current_buffer_id);
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
