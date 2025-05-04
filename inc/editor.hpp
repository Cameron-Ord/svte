#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef enum
{
    VISUAL = 0,
    INSERT = 1,
    REPLACE = 2,
    APPEND = 3,
    CMD = 4
} MODES;

typedef enum
{
    NEXT_BUFFER = 3,
    PREV_BUFFER = 4,

    DEL = 5,
    RMV = 6,
    INS = 7,
    APND = 37,

    C_NEWLINE = 36,

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

typedef enum
{
    FILE_RET_NOEXIST = 14,
    FILE_RET_ERR = 15,
    FILE_RET_OK = 16,
    FILE_RET_NO_FN = 29
} FILE_RET;

typedef enum
{
    STR_CONCAT_OK = 21,
    STR_CONCAT_NO_FN = 22
} STR_RET;

typedef enum
{
    BAD_SIZE = 30,
    DEL_OK = 31,
    BAD_APND = 40,
    INS_OK = 33,
    RMV_OK = 34,
    ROW_OK = 35,
    BAD_INS = 39,
    BAD_DEL = 44
} INS_RET;

typedef enum
{
    FILE_ID_BROKEN = -1,
} FILE_ID_STATE;

typedef enum
{
    BUF_STATE_VALID = 25,
    BUF_STATE_ERR = 26
} BUFFER_STATE;

struct Line
{
    std::string str;
    int col;
};
typedef struct Line Line;

class Buffer
{
  public:
    Buffer(std::string filename, std::string subpath, const int identifier);
    const std::vector<Line> *buf_get_buffer(void);
    const int *buf_get_row(void);
    const int *buf_get_col(void);
    const int32_t buf_get_id(void){ return id; }
    int buf_save_file(void);
    int buf_open_file(void);
    int buf_concat_paths(void);
    int buf_row_first_char(std::string row, const int offset);
    int buf_row_insert_char(const char c);
    int buf_row_append_char(const char c);
    int buf_row_remove_char(void);
    int buf_row_append(void);
    int buf_row_delete_char(void);
    void buf_shift_curs_col(const int d);
    void buf_shift_curs_row(const int d);
    int buf_get_valid(void);

  private:
    int is_valid;
    int row;
    int filename_change_flag;
    std::vector<Line> buffer;
    std::string filename;
    std::string subpath;
    std::string fullpath;
    std::string path_delimiter;
    int32_t id;
};

class Editor
{
  public:
    Editor(std::string spath);

    void ed_append_cmd(const char c);
    void ed_del_cmd(void);
    void ed_enter_cmd(void);
    void ed_append_buffer(std::string filename);
    //void ed_destroy_buffer(void);
    //void ed_buffers_resize(void);
    //void ed_buffers_shift(void);
    //void ed_rmv_char(void);
    //void ed_del_char(void);
    void ed_mv_op(const int DIRECTION);
    void ed_str_op(const int OPERATION, const char c);
    //void ed_chng_buffer(void);
    //void ed_name_find_buffer(void);
    //void ed_chng_mode(void);
    int ed_get_mode(void) { return editor_mode; }
    void ed_set_mode(const int mode) { editor_mode = mode; }
    int ed_save_buffer(void);
    int32_t ed_gen_id(void);
    Buffer *ed_grab_buffer(void);
    std::vector<int32_t>::iterator  ed_buffer_get_iter(void);
    void ed_swap_buffer(const uint32_t id);
    int32_t ed_find_next_buffer(void);
    void ed_change_buffer(const int ARG);

  private:
    std::string cmd, editor_subpath;
    std::unordered_map<int32_t, Buffer *> bufs;
    std::unordered_set<int32_t> ids;
    std::vector<int32_t> open_buffers;
    int32_t current_buffer;
    int editor_mode;
};

#endif
