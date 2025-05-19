#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cstdint>


struct EditorCmd{
    std::string cmdstr;
    int cursor;
};

//Forward declare the buffer class, must be defined in SRC files.
class Buffer;

typedef enum { SELECT = 0, INSERT = 1, CMD = 2, NAV = 3, APPEND = 4 } MODES;

class Editor {
    public:
        Editor(void);
        ~Editor(void);
        
        int32_t ed_gen_id(void);
        int ed_no_file(const int32_t id);
        int ed_open_file(const int32_t id);
        int ed_save_file(const int32_t id);
        void ed_set_current_id(const int32_t id);
        int ed_append_buffer(std::string fn);
        int ed_append_buffer(void);
        void ed_set_error(const int err);
        int ed_determine_cwd(void);
        void ed_set_cwd(std::string str);
        std::string ed_fs_cwd_string(void);
        std::string ed_delimiter(void);
        uint8_t ed_get_mode(void);
        void ed_set_mode(const uint8_t val);

        Buffer *ed_fetch_buffer(const int32_t id);
        const Buffer* const ed_fetch_buffer_const(const int32_t id) const;
        const std::string& ed_get_cwd(void);
        const std::vector<int32_t>& ed_get_open(void);
        const int32_t& ed_get_current_id(void);
        const int& ed_get_error(void);
        const int32_t ed_commit_buffer(std::string fn);
        
        Editor& ed_eval_cmd(void);
        void ed_do_cmd(void);
        void ed_cmd_ins(const unsigned char c);
        
    private:
        int error;
        uint8_t mode;
        int32_t current_buffer_id;
        std::string ed_working_path;
        std::unordered_map<int32_t, Buffer*> bufs;
        std::unordered_set<int32_t> used_ids;
        std::vector<int32_t> open;
        EditorCmd cmd;
};

#endif