#include "../inc/editor.hpp"
#include "../inc/defines.hpp"
#include <iostream>

Editor::Editor(std::string spath)
{
    bufs.clear();
    ids.clear();
    open_buffers.clear();
    cmd.clear();
    current_buffer = 0;
    editor_mode = VISUAL;
    editor_subpath = spath;
}

std::vector<int32_t>::iterator Editor::ed_buffer_get_iter(void){
    std::cout << "Buffer ID before: " << current_buffer << std::endl;
    std::vector<int32_t>::iterator it;
    for(it = open_buffers.begin(); it != open_buffers.end(); ++it){
        if(*it == current_buffer){
            return it;
        }
    }
    // Not found.
    return open_buffers.end();
}

int32_t Editor::ed_find_next_buffer(void){
    std::vector<int32_t>::iterator it = ed_buffer_get_iter();

    //Loop around to beginning if necessary
    if(it == open_buffers.end() || it + 1 == open_buffers.end()){
        it = open_buffers.begin();
    }

    for(; it != open_buffers.end(); ++it){
        if(*it != current_buffer){
            std::cout << "Returned: iterator value:  " << *it << std::endl;
            return *it;
        }
    }
    //Not found.
    std::cout << "Returned: " << current_buffer << std::endl;
    return current_buffer;
}

void Editor::ed_change_buffer(const int ARG){
    std::cout << "Available buffers: " << open_buffers.size() << std::endl;
    switch(ARG){
        default: break;
        case NEXT_BUFFER:{
            ed_swap_buffer(ed_find_next_buffer());
        }break;
    }
}

void Editor::ed_swap_buffer(const uint32_t id){
    current_buffer = id;
}

void Editor::ed_append_cmd(const char c){
    cmd.push_back(c);
}

void Editor::ed_del_cmd(void){
    cmd.clear();
}

int Editor::ed_enter_cmd(void){
    std::string::iterator it;
    for(it = cmd.begin(); it != cmd.end(); ++it){
        if(*it == SPACECHAR){
            cmd.erase(it);
        }
    }
    if(cmd.substr(0, 3) != "new"){
        return CMD_STATE_FAIL;
    }

    std::string filename = cmd.substr(3, cmd.size());
    if(ed_append_buffer(filename) != BUF_STATE_VALID){
        return CMD_STATE_FAIL;
    }
    std::cout << "Buffer: " << filename << " Added" << std::endl;
    ed_del_cmd();
    return CMD_STATE_OK;
}

int Editor::ed_save_buffer(void){
    Buffer *buf = nullptr;
    if (!(buf = ed_grab_buffer())) {
        return 0;
    }
    if(buf->buf_save_file() != FILE_RET_OK){
        return 0;
    }
    return 1;
}

Buffer *Editor::ed_grab_buffer(void)
{
    std::unordered_map<int32_t, Buffer *>::iterator it = bufs.find(current_buffer);
    if (it != bufs.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

int32_t Editor::ed_gen_id(void)
{
    int attempt = 0;
    const int max_attempts = 100;
    std::unordered_set<int32_t>::iterator it;

    while (attempt < max_attempts) {
        //random n between 0 and int32 max
        const uint32_t generated = rand() % UINT32_MAX;
        it = ids.find(generated);
        if (it == ids.end()) {
            ids.insert(generated);
            return generated;
        }
        attempt++;
    }
    //return a negative id
    return FILE_ID_BROKEN;
}

int Editor::ed_append_buffer(std::string filename)
{
    int32_t id = ed_gen_id();
    Buffer *buf = new (std::nothrow) Buffer(filename, editor_subpath, id);
    if (!buf) {
        std::cerr << "Failed to create buffer! (bad alloc)" << std::endl;
        return;
    }
    if(buf->buf_get_valid() == BUF_STATE_ERR){
        std::cerr << "Buffer created with errors!" << std::endl;
        if(buf->buf_get_id() != FILE_ID_BROKEN){
            ids.erase(id);
        }
        delete buf;
        return;
    }
    bufs.insert({id, buf});
    current_buffer = id;
    open_buffers.push_back(id);
    std::cout << "external buffer ID: " << id << std::endl;
}

void Editor::ed_str_op(const int OPERATION, const char c)
{
    Buffer *buf = nullptr;
    if (!(buf = ed_grab_buffer())) {
        return;
    }

    switch (OPERATION) {
    default:
        break;
    case C_NEWLINE:
    {
        if (buf->buf_row_append() != ROW_OK) {
            std::cerr << "Failed to append row!" << std::endl;
            return;
        }
    } break;
    case DEL:
    {
        if (buf->buf_row_delete_char() != DEL_OK) {
            std::cerr << "Failed to delete character!" << std::endl;
            return;
        }
    } break;
    case RMV:
    {
        if (buf->buf_row_remove_char() != RMV_OK) {
            std::cerr << "Failed to remove character!" << std::endl;
            return;
        }
    } break;
    case INS:
    {
        if (buf->buf_row_insert_char(c) != INS_OK) {
            std::cerr << "Failed to insert character!" << std::endl;
            return;
        }
    } break;

    case APND:
    {
        if (buf->buf_row_append_char(c) != INS_OK) {
            std::cerr << "Failed to append character!" << std::endl;
            return;
        }
    } break;
    }
}

void Editor::ed_mv_op(const int DIRECTION)
{
    Buffer *buf = nullptr;
    if (!(buf = ed_grab_buffer())) {
        return;
    }

    switch (DIRECTION) {
    default:
        break;
    case MV_LEFT:
    {
        buf->buf_shift_curs_col(-1);
    } break;
    case MV_RIGHT:
    {
        buf->buf_shift_curs_col(1);
    } break;
    case PREV_LINE:
    {
        buf->buf_shift_curs_row(-1);
    } break;
    case NEXT_LINE:
    {
        buf->buf_shift_curs_row(1);
    } break;
    }
}

//void switch_buffer(Editor *e, const int direction)
//{
//}
//
//static int del_char_force_bounds(const size_t bs, const int pos)
//{
//if (pos >= (int)bs) {
//return 0;
//}
//
//if (pos < 0) {
//return 0;
//}
//
//return 1;
//}
//
//int buffer_del_char(Editor *e)
//{
//const Buf *b = e->buffers->get_buf(e->current_buffer);
//if (b->bchar.size < 1) {
//return 0;
//}
//
//if (e->buffers->buf_bounds(e->current_buffer)) {
//if (!del_char_force_bounds(b->bchar.size, b->bcurs.pos)) {
//e->buffers->buf_pos_bw(e->current_buffer);
//}
//
//e->buffers->shift_buffer(DEL, e->current_buffer);
//e->buffers->buf_realloc(e->current_buffer, b->bchar.size - 1);
//}
//
//return 1;
//}
////
//int buffer_rm_char(Editor *e)
//{
//const Buf *b = e->buffers->get_buf(e->current_buffer);
//if (!(b->bcurs.pos > 0) || b->bchar.size < 1)
//return 0;
//
//if (e->buffers->buf_bounds(e->current_buffer)) {
//e->buffers->shift_buffer(RMV, e->current_buffer);
//e->buffers->buf_pos_bw(e->current_buffer);
//e->buffers->buf_realloc(e->current_buffer, b->bchar.size - 1);
//}
//
//return 1;
//}
////
//int buffer_insert_char(Editor *e, const char c)
//{
//const Buf *b = e->buffers->get_buf(e->current_buffer);
//if (e->buffers->buf_bounds(e->current_buffer)) {
//e->buffers->buf_realloc(e->current_buffer, b->bchar.size + 1);
//e->buffers->shift_buffer(INS, e->current_buffer);
//e->buffers->buf_insert(e->current_buffer, &c);
//}
//return 1;
//}
////
//void buffer_mv_op(Editor *e, const int op)
//{
//e->buffers->buf_mv_pos(e->current_buffer, op);
//}
