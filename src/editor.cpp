#include "../inc/editor.hpp"
#include <cstdlib>

Editor::Editor(std::string wpath, std::string str_arg)
    : bufs(Buffers(wpath, str_arg)) {
  grid.row.resize(1);
  grid.col.resize(1);
  pos.x = 0, pos.y = 0;
}
