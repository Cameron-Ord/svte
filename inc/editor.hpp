#ifndef EDITOR_HPP
#define EDITOR_HPP
#include "vecdef.hpp"

struct Column {
  int width;
  int locn;
};
typedef struct Column Column;

struct Row {
  int height;
  int locn;
};
typedef struct Row Row;

// Dynamically allocate these so rows and columns can be added/removed
struct Editor {
  Row *rows;
  Column *cols;
  Vec2i pos;
};
#endif
