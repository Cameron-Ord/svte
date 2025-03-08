#include "../inc/editor.hpp"
#include <cstdlib>

Editor::Editor(std::string wpath, std::string str_arg)
    : bufs(Buffers(wpath, str_arg)) {
  pos.x = 0, pos.y = 0;
  curr_buffer_i = 0;
}

void Editor::zero_grid(Vec2i grid_vec) {
  grid.w = grid_vec.x, grid.h = grid_vec.y;
  grid.col_count = 0;
  grid.row_count = 0;
}
