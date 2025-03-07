#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "grid.hpp"
#include "vecdef.hpp"

#include <cstdio>
#include <string>

struct Buf {
  std::string fn;
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

  size_t buf_realloc(const size_t i, File_Info fs);
  void delete_buffer(std::string file_name);
  void append_buffer(std::string file_name);
  int match_buffer(std::string key);
  int write_buffer(std::string file_name);

  std::string current_file(const size_t idx) { return buffers[idx].fn; }
  void set_buf_index(const size_t i) { buf_index = i; }
  int get_buf_index(void) { return buf_index; }

  void update_working_path(std::string overwrite) { working_path = overwrite; }
  std::string get_working_path(void) { return working_path; }
  std::string strjoin(std::string base, std::string append) {
    return base + "/" + append;
  }

private:
  std::string working_path;
  size_t buf_index;
  std::vector<Buf> buffers;
};

class Editor {
public:
  Editor(std::string path, std::string str_arg);
  Buffers *_bufs(void) { return &bufs; }

private:
  Buffers bufs;
  Grid grid;
  Vec2i pos;
};

#endif
