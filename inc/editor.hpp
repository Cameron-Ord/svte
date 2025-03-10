#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <vector>

#include "vecdef.hpp"

#include <cstdio>
#include <string>

struct Buf {
  std::string fn;
  int fn_needs_change;
  char *buf;
  char **split;
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
  int buf_split_by_nl(const size_t i);
  size_t buf_malloc(const size_t i, const size_t size);
  size_t buf_realloc(const size_t i, const size_t new_size);
  void delete_buffer(std::string fn);
  void append_buffer(std::string fn, const int fn_needs_change);
  int match_buffer(std::string key);
  int write_buffer(std::string fn);
  size_t read_file(std::string fn);
  size_t buffer_count(void) { return buffers.size(); }
  const Buf *get_buf(const size_t i);
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
  size_t _buf_i(void) { return curr_buffer_i; }
  Vec2i *_cursor(void) { return &cursor; }

private:
  size_t curr_buffer_i;
  Buffers bufs;
  Vec2i cursor;
};

#endif
