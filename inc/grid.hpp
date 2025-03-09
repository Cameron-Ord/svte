#ifndef GRID_HPP
#define GRID_HPP
#include <string>

struct Grid {
  // The corresponding filename for the specified grid data.
  std::string fn;
  int w, h;
  int col_count;
  int row_count;
};
typedef struct Grid Grid;
#endif
