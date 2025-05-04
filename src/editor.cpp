#include "../inc/editor.hpp"
#include <iostream>

Editor::Editor(void)
{
    bufs.clear();
    ids.clear();
    open_buffers.clear();
    current_buffer = 0;
    editor_mode = VISUAL;
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

void Editor::ed_append_buffer(std::string filename, std::string subpath)
{
    int32_t id = ed_gen_id();
    Buffer *buf = new (std::nothrow) Buffer(filename, subpath, id);
    if (!buf) {
        std::cerr << "Failed to create buffer! (bad alloc)" << std::endl;
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
