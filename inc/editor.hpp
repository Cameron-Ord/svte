#ifndef EDITOR_HPP
#define EDITOR_HPP
#include "vecdef.hpp"
#include <cstdio>
#include <vector>

struct String {
  char *str;
  size_t len;
  size_t locn;
};
typedef struct String String;

struct Buf {
  char *fn;
  char *buf;
  // lines[rows][cols]
  int fn_needs_change;
  String *lines;
  size_t lcount;
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
  Buffers(char *pathstr, char *arg_str);

  int line_realloc(const size_t i, const size_t ns, const size_t pos);
  int line_alloc(const size_t i);
  void print_file(const int i);
  char *random_fn(void);
  int buf_split_by_nl(const size_t i);
  size_t buf_malloc(const size_t i, const size_t size);
  size_t buf_realloc(const size_t i, const size_t new_size);
  void delete_buffer(const char *fn);
  void append_buffer(char *fn, const int fn_needs_change);
  int match_buffer(const char *key);
  int write_buffer(const char *fn);
  size_t read_file(const char *fn);
  size_t buffer_count(void) { return buffers.size(); }
  const Buf *get_buf(const size_t i);
  void update_working_path(char *str) { working_path = str; }
  const char *get_working_path(void) { return working_path; }

private:
  // Mutable working path pointer
  char *working_path;
  std::vector<Buf> buffers;
};

class Editor {
public:
  Editor(char *pathstr, char *arg_str);
  Buffers *_bufs(void) { return &bufs; }
  size_t _buf_i(void) { return curr_buffer_i; }
  Vec2i *_cursor(void) { return &cursor; }
  int buffer_insert_char(const unsigned char);
  int buffer_rm_char(const unsigned char c);

private:
  size_t curr_buffer_i;
  Buffers bufs;
  Vec2i cursor;
};

#endif
