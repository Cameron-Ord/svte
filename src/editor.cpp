#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>

Editor::Editor(char *pathstr, char *arg_str) : bufs(Buffers(pathstr, arg_str)) {
  curr_buffer_i = 0;
}

int Editor::buffer_rm_char(const unsigned char c) {
  const size_t p = bufs.get_buf_pos(curr_buffer_i);
}
int Editor::buffer_insert_char(const unsigned char c) {
  const size_t p = bufs.get_buf_pos(curr_buffer_i);
}
void Editor::buffer_set_position(const size_t i) {
  bufs.set_buf_pos(curr_buffer_i, i);
}
