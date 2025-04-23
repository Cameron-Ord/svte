#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <cstdio>
#include <cstdint>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

// Representing operations as integers inside an enum.
// Makes things straightforward and easy to understand/program. Which is cool
// and good

struct Cursor {
    int row, col;
    int w, h;
};
typedef struct Buffer_Cursor Buffer_Cursor;

struct File_Info
{
    long fs;
    FILE *f;
};
typedef struct File_Info File_Info;


 //class Buffers
 //{
   //public:
     //Buffers(char *pathstr, char *arg_str);
     ////void calculate_buffer_height(const int i); 
     ////void calculate_cursor_positions(const int i);
     //int allocate_buffer_list(void);
     //int realloc_buffer_list(const int direction);
     //int get_text_height(const int i);
     //int get_cursor_height(const int i);
     //int find_word(const int i, const int direction);
     //int find_line(const int i, const int direction);
     //int buf_bounds(const int i);
     //int pos_bounds(const int i, const int pos);
     //void buf_replace_at(const int i, const unsigned char c);
     //void buf_pos_bw(const int i);
     //void buf_insert(const int i, const char *c);
     //void shift_buffer(const int direction, const int pos);
     //void print_file(const int i);
     //char *random_fn(void);
     //size_t buf_malloc(const int i, const size_t size);
     //size_t buf_realloc(const int i, const size_t new_size);
     //void delete_buffer(const int i);
     //void append_buffer(char *wp, char *fn, const int fn_needs_change);
     //int match_buffer(const char *key);
     //int write_buffer(const char *wp, const char *fn);
     //size_t read_file(const char *wp, const char *fn);
     //size_t buffer_count(void);
     //const Buf *get_buf(const int i);
     //void buf_mv_pos(const int i, const int operation);
     //const char *get_working_path(const int i);
     //void update_cursor_dimensions(const int i, const Vec2i *dims);
 //
   //private:
     //Buf *buffers;
     //size_t buf_count;
 //};

typedef enum
{
    VISUAL = 0,
    INSERT = 1,
    REPLACE = 2
} MODES;

typedef enum
{
    NEXT_BUFFER = 3,
    PREV_BUFFER = 4,

    DEL = 5,
    RMV = 6,
    INS = 7,

    MV_RIGHT = 8,
    MV_LEFT = 9,

    NEXT_WORD = 10,
    PREV_WORD = 11,
    PREV_LINE = 12,
    NEXT_LINE = 13,
    // not yet
    // COPY_AT = 7,
    // PASTE_AT = 8,
} OPERATIONS;


typedef enum {
    FILE_RET_NOEXIST = 14,
    FILE_RET_ERR = 15,
    FILE_RET_OK = 16
}FILERET;

typedef enum {
    BUF_READ_OK = 17,
    BUF_READ_ERR = 18
}READRET;

typedef enum {
    STR_DUP_OK = 19,
    STR_DUP_ERR = 20
}DUPERET;

typedef enum {
    STR_CONCAT_OK = 21,
    STR_CONCAT_ERR = 22
}CONCATRET;

typedef enum {
    BUF_ALLOC_OK = 23,
    BUF_ALLOC_ERR = 24
}ALLOCRET;

typedef enum { FILE_ID_BROKEN = -1, }FILE_ID_STATE;

typedef enum {BUF_STATE_VALID = 25, BUF_STATE_ERR = 26}BUFFER_STATE;

typedef enum { SPLIT_BUF_ERR = 27, SPLIT_BUF_OK = 28 } SPLITRET;
class Buffer {
    public:
        Buffer(char *filename, char *subpath, const int identifier);
        std::vector<std::string> *buf_get_buffer(void) { return &buffer; } 
        const Cursor *buf_get_curs(void) { return &cursor; }
        int buf_dupe_paths(char *filename, char *subpath);
        int buf_concat_path(void);
        int buf_open_file(void);
        int buf_raw_allocate(const size_t bsize);
        int buf_raw_destroy(void);
        int buf_raw_read(void);
        int buf_split_buffer(void);
        int buf_shift_curs_x(const int d);
        int buf_shift_curs_y(const int d);
        int buf_clamp_col(const int col, const int ssize);
         //int buf_resize(void);
         //int buf_shift(void);
         //int buf_ins_char(void);
         //int buf_rmv_char(void);
         //int buf_mv_cursor(void);
         //int buf_del_char(void);
         //int buf_mv_find_line(void);
         //int buf_mv_find_word(void);
        int buf_get_valid(void) { return valid_buffer; }

    private:
        int32_t id;
        int valid_buffer;
        FILE *file;
        // Using stdvec of strings because its generally just safer and easier
        // to use than char[rows][cols]
        std::vector<std::string> buffer;
        // raw buffer is used when opening and saving files.
        char *raw_buffer;
        char *filename;
        char *subpath;
        char *fullpath;
        size_t buffer_size;
        size_t buffer_size_max;
        long file_size_at_open;
        Cursor cursor;
        size_t filename_str_len;
        size_t subpath_str_len;
        int filename_change_flag;
};

class Editor {
    public:
        Editor(void);
        
        void ed_append_buffer(char* filename, char *subpath);
        void ed_destroy_buffer(void);
        void ed_buffers_resize(void);
        void ed_buffers_shift(void);
        void ed_rmv_char(void);
        void ed_del_char(void);
        void ed_ins_char(void);
        void ed_mv_op(const int DIRECTION);
        void ed_chng_buffer(void);
        void ed_name_find_buffer(void);
        void ed_chng_mode(void);
        int  ed_get_mode(void) { return editor_mode; } 
        int32_t  ed_gen_id(void);
        Buffer *ed_grab_buffer(void);
        

    private:
        std::unordered_map<int32_t, Buffer> bufs;
        std::unordered_set<int32_t> ids;
        std::vector<int32_t> open_buffers;
        int32_t current_buffer;
        int editor_mode;
};

 //struct Editor
 //{
     //int mode;
     //int current_buffer;
     //int open_buffers;
     //Buffers *buffers;
     //int (*del)(Editor *);
     //int (*rm)(Editor *);
     //int (*ins)(Editor *, const char);
     //void (*mv)(Editor *, const int);
     //void (*sb)(Editor *, const int);
 //};

typedef struct Editor Editor;
#endif
