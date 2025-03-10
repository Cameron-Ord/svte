#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>

Editor::Editor(char *pathstr, char *arg_str) : bufs(Buffers(pathstr, arg_str)) {
  cursor.x = 0, cursor.y = 0;
  curr_buffer_i = 0;
}

int Editor::buffer_rm_char(const unsigned char c) {}

int Editor::buffer_insert_char(const unsigned char c) {}
