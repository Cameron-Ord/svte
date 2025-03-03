#ifndef GRID_HPP
#define GRID_HPP
#include <vector>

struct Column {
  int width;
};
typedef struct Column Column;

struct Row {
  int height;
};
typedef struct Row Row;

struct Grid {
  std::vector<Column> col;
  std::vector<Row> row;
  int col_count;
  int row_count;
};
typedef struct Grid Grid;
#endif
