#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <cstdio>
#include <vector>

#define NEWLINE '\n'

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

  NEXT_WORD_FIRST = 3,
  PREV_WORD_FIRST = 4,
  PREV_LINE_FIRST = 5,
  NEXT_LINE_FIRST = 6,

  NEXT_WORD_LAST = 7,
  PREV_WORD_LAST = 8,
  PREV_LINE_LAST = 9,
  NEXT_LINE_LAST = 10,

  // not yet
  // COPY_AT = 7,
  // PASTE_AT = 8,
} OPERATIONS;

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
  size_t size;
  size_t pos;
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

  int bounds(const int i);
  void buf_rm(const size_t i);
  void buf_insert(const size_t i, unsigned char c);
  void shift_buffer(const int direction, const size_t pos);
  void print_file(const int i);
  char *random_fn(void);
  size_t buf_malloc(const size_t i, const size_t size);
  size_t buf_realloc(const size_t i, const size_t new_size);
  void delete_buffer(const char *fn);
  void append_buffer(char *fn, const int fn_needs_change);
  int match_buffer(const char *key);
  int write_buffer(const char *fn);
  size_t read_file(const char *fn);
  size_t buffer_count(void) { return buffers.size(); }
  const Buf *get_buf(const size_t i);
  void buf_mv_pos(const size_t i, const int operation);
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
  size_t _buf_i(void) { return buf_i; }
  int buffer_insert_char(const unsigned char c);
  int buffer_rm_char(void);
  int buffer_del_char(void);
  void buffer_mv_op(const int operation);
  void switch_buffer(const int direction);

private:
  size_t buf_i;
  Buffers bufs;
};

#endif
