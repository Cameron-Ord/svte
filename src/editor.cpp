#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>

Editor::Editor(char *pathstr, char *arg_str) : bufs(Buffers(pathstr, arg_str)) {
  curr_buffer_i = 0;
}

int Editor::buffer_rm_char(const unsigned char c) {}
int Editor::buffer_insert_char(const unsigned char c) {
  const Buf *b = bufs.get_buf(curr_buffer_i);
  bufs.buf_realloc(curr_buffer_i, b->size + 1);
  bufs.shift_buffer(1, curr_buffer_i);
  bufs.buf_insert(curr_buffer_i, c);
  return 1;
}
void Editor::buffer_set_position(const size_t i) {
  bufs.set_buf_pos(curr_buffer_i, i);
}
