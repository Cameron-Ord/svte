#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "grid.hpp"
#include "vecdef.hpp"

#include <cstdio>
#include <string>

struct Buf {
  std::string fn;
  int fn_needs_change;
  char *buf;
  size_t size;
};
typedef struct Buf Buf;

struct File_Info {
  long fs;
  FILE *f;
};
typedef struct File_Info File_Info;

class Buffers {
public:
  Buffers(std::string wpath, std::string str_arg);

  void print_file(const int i);
  std::string random_fn(void);
  size_t buf_malloc(const size_t i, const size_t size);
  size_t buf_realloc(const size_t i, const size_t new_size);
  void delete_buffer(std::string fn);
  void append_buffer(std::string fn, const int fn_needs_change);
  int match_buffer(std::string key);
  int write_buffer(std::string fn);
  size_t read_file(std::string fn);
  const Buf *cur_buffer(const size_t i) { return &buffers[i]; }
  void update_working_path(std::string overwrite) { working_path = overwrite; }
  std::string get_working_path(void) { return working_path; }
  std::string strjoin(std::string base, std::string append) {
    return base + "/" + append;
  }

private:
  std::string working_path;
  std::vector<Buf> buffers;
};

class Editor {
public:
  Editor(std::string path, std::string str_arg);
  Buffers *_bufs(void) { return &bufs; }
  size_t get_cur_buf(void) { return curr_buffer_i; }

  void grid_inc_rows(void);
  void grid_decrm_rows(void);

  void grid_inc_cols(void);
  void grid_decrm_cols(void);

  void zero_grid(Vec2i grid_vector);

private:
  size_t curr_buffer_i;
  Buffers bufs;
  Grid grid;
  Vec2i pos;
};

#endif
