#include "../inc/editor.hpp"
#include <cassert>
#include <cstdlib>

Editor::Editor(std::string wpath, std::string str_arg)
    : bufs(Buffers(wpath, str_arg)) {
  pos.x = 0, pos.y = 0;
  curr_buffer_i = 0;
}
