#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>

Editor::Editor(char *pathstr, char *arg_str) : bufs(Buffers(pathstr, arg_str)) {
  cursor.x = 0, cursor.y = 0;
  curr_buffer_i = 0;
}
