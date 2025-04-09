#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <cstdio>
#include <vector>
#include "vecdef.hpp"

// Representing operations as integers inside an enum.
// Makes things straightforward and easy to understand/program. Which is cool
// and good
typedef enum
{
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

struct String
{
    char *str;
    size_t len;
    int locn;
};
typedef struct String String;

struct Buffer_Cursor {
    int pos;
    int rowy, rowx;
    int w, h;
};
typedef struct Buffer_Cursor Buffer_Cursor;

struct Buffer_Char { 
    char *fn;
    char *working_path;
    char *buf;
    int fn_needs_change;
    size_t size;
};
typedef struct Buffer_Char Buffer_Char;

struct Buffer_Dims {
    int text_height;
    int rows, cols;
    int pos_offset_y, pos_offset_x;
};
typedef struct Buffer_Dims Buffer_Dims;


struct Buf
{
    Buffer_Char bchar;
    Buffer_Cursor bcurs;
    Buffer_Dims bdims;
};
typedef struct Buf Buf;

struct File_Info
{
    long fs;
    FILE *f;
};
typedef struct File_Info File_Info;


class Buffers
{
  public:
    Buffers(char *pathstr, char *arg_str);
    //void calculate_buffer_height(const int i); 
    //void calculate_cursor_positions(const int i);
    int allocate_buffer_list(void);
    int realloc_buffer_list(const int direction);
    int get_text_height(const int i);
    int get_cursor_height(const int i);
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
    void delete_buffer(const int i);
    void append_buffer(char *wp, char *fn, const int fn_needs_change);
    int match_buffer(const char *key);
    int write_buffer(const char *wp, const char *fn);
    size_t read_file(const char *wp, const char *fn);
    size_t buffer_count(void);
    const Buf *get_buf(const int i);
    void buf_mv_pos(const int i, const int operation);
    const char *get_working_path(const int i);
    void update_cursor_dimensions(const int i, const Vec2i *dims);

  private:
    Buf *buffers;
    size_t buf_count;
};

typedef enum
{
    VISUAL = 0,
    INSERT = 1,
    REPLACE = 2
} MODES;

struct Editor
{
    int mode;
    int current_buffer;
    int open_buffers;
    Buffers *buffers;
    int (*del)(Editor *);
    int (*rm)(Editor *);
    int (*ins)(Editor *, const char);
    void (*mv)(Editor *, const int);
    void (*sb)(Editor *, const int);
};

void switch_buffer(Editor *e, const int direction);
int buffer_del_char(Editor *e);
int buffer_rm_char(Editor *e);
int buffer_insert_char(Editor *e, const char c);
void buffer_mv_op(Editor *e, const int op);

typedef struct Editor Editor;
#endif
