#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "grid.hpp"
#include "vecdef.hpp"

#include <cstdio>
#include <string>

struct Buf {
  char *buf;
  size_t size;
};
typedef struct Buf Buf;

class Buffers {
public:
  Buffers(void);
  FILE *read_file(void);
  int write_file(void);

private:
  FILE *f;
  std::vector<Buf> buffers;
  size_t buffer_count;
};

class Editor {
public:
  Editor(void);

  std::vector<std::string> get_base_paths(void) { return paths; }
  std::vector<std::string> get_file_names(void) { return file_names; }
  std::vector<std::string> get_abs_paths(void) { return abs_paths; }

private:
  Buffers bufs;
  std::vector<std::string> paths;
  std::vector<std::string> file_names;
  std::vector<std::string> abs_paths;
  Grid grid;
  Vec2i pos;
};
#endif
