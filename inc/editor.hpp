#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <cstdio>
#include <vector>

// Representing operations as integers inside an enum.
// Makes things straightforward and easy to understand/program. Which is cool
// and good
typedef enum {
  NEXT_BUFFER = 0,
  PREV_BUFFER = 1,

  DEL = 0,
  RMV = -1,
  INS = 1,

  MV_RIGHT = 1,
  MV_LEFT = 2,

  NEXT_WORD = 3,
  PREV_WORD = 4,
  PREV_LINE = 5,
  NEXT_LINE = 6,

  // not yet
  // COPY_AT = 7,
  // PASTE_AT = 8,
} OPERATIONS;

struct String {
  char *str;
  size_t len;
  int locn;
};
typedef struct String String;

struct Buf {
  char *fn;
  char *buf;
  // lines[rows][cols]
  int fn_needs_change;
  size_t size;
  int pos;
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

  int find_word(const int i, const int direction);
  int find_line(const int i, const int direction);
  int buf_bounds(const int i);
  int pos_bounds(const int i, const int pos);
  void buf_replace_at(const int i, const unsigned char c);
  void buf_pos_bw(const int i);
  void buf_insert(const int i, const char *c);
  void shift_buffer(const int direction, const int pos);
  void print_file(const int i);
  char *random_fn(void);
  size_t buf_malloc(const int i, const size_t size);
  size_t buf_realloc(const int i, const size_t new_size);
  void delete_buffer(const char *fn);
  void append_buffer(char *fn, const int fn_needs_change);
  int match_buffer(const char *key);
  int write_buffer(const char *fn);
  size_t read_file(const char *fn);
  size_t buffer_count(void) { return buffers.size(); }
  const Buf *get_buf(const int i);
  void buf_mv_pos(const int i, const int operation);
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
  int _buf_i(void) { return buf_i; }
  int buffer_insert_char(const char c);
  int buffer_rm_char(void);
  int buffer_del_char(void);
  void buffer_mv_op(const int operation);
  void switch_buffer(const int direction);

private:
  int buf_i;
  Buffers bufs;
};

#endif
