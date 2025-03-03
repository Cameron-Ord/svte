#include "../inc/editor.hpp"
#include <cstdlib>

Editor::Editor(void) : bufs(Buffers()) {
  paths.clear();
  file_names.clear();
  abs_paths.clear();
  grid.row.resize(1);
  grid.col.resize(1);
  pos.x = 0, pos.y = 0;
}
