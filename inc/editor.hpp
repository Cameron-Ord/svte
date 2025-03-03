#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "grid.hpp"
#include "vecdef.hpp"

#include <cstdio>
#include <string>

// Dynamically allocate these so rows and columns can be added/removed
class Editor {
public:
  Editor(std::string path, std::string file);

private:
  FILE *f;
  char *buffer;
  std::string current_path;
  std::string file_str;
  std::string abs_path;
  Grid grid;
  Vec2i pos;
};
#endif
