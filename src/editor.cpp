#include "../inc/editor.hpp"

#include <cassert>
#include <cstdlib>
#include <cstring>

Editor::Editor(void){
    bufs.clear();
    buffer_count = 0;
    current_buffer = 0;
    editor_mode = VISUAL;
}

void Editor::ed_append_buffer(const char* filename, const char *subpath){
    
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
