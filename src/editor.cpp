#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>

Editor::Editor(std::string wpath, std::string str_arg)
    : bufs(Buffers(wpath, str_arg)) {
  cursor.x = 0, cursor.y = 0;
  curr_buffer_i = 0;
}
