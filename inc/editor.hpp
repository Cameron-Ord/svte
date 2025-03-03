#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "grid.hpp"
#include "vecdef.hpp"

// Dynamically allocate these so rows and columns can be added/removed
struct Editor {
  Grid grid;
  Vec2i pos;
};
#endif
