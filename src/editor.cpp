#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

Editor::Editor(char *pathstr, char *arg_str) : bufs(Buffers(pathstr, arg_str)) {
  curr_buffer_i = 0;
}

int Editor::buffer_del_char(void) {
  const Buf *b = bufs.get_buf(curr_buffer_i);
  if (b->pos >= 0 && b->pos < b->size - 1) {
    bufs.shift_buffer(DEL, curr_buffer_i);
    bufs.buf_realloc(curr_buffer_i, b->size - 1);
  }
  return 1;
}

int Editor::buffer_rm_char(void) {
  const Buf *b = bufs.get_buf(curr_buffer_i);
  if (b->pos > 0 && b->pos <= b->size - 1) {
    bufs.shift_buffer(RMV, curr_buffer_i);
    bufs.buf_rm(curr_buffer_i);
    bufs.buf_realloc(curr_buffer_i, b->size - 1);
  }
  return 1;
}

int Editor::buffer_insert_char(const unsigned char c) {
  const Buf *b = bufs.get_buf(curr_buffer_i);
  if (b->pos <= b->size - 1) {
    bufs.buf_realloc(curr_buffer_i, b->size + 1);
    bufs.shift_buffer(INS, curr_buffer_i);
    bufs.buf_insert(curr_buffer_i, c);
  }
  return 1;
}

void Editor::buffer_mv_position(const int direction) {
  bufs.buf_mv_pos(curr_buffer_i, direction);
}
