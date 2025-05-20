#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cstdint>


struct EditorCmd{
    std::string cmdstr;
    std::string sanitized;
    int cursor;
};

//Forward declare the buffer class, must be defined in SRC files.
class Buffer;

typedef enum { SELECT = 0, INSERT = 1, CMD = 2, NAV = 3, APPEND = 4 } MODES;

class Editor {
    public:
        Editor(void);
        ~Editor(void);
        
        int32_t ed_next_id(void);
        int32_t ed_prev_id(void);
        int32_t ed_gen_id(void);

        void ed_set_current_id(const int32_t id);
        int ed_append_buffer(std::pair<std::string, std::vector<std::string>> commit);
        void ed_set_error(const int err);
        uint8_t ed_get_mode(void) const { return mode; }
        void ed_set_mode(const uint8_t val);

        Buffer *ed_fetch_buffer(const int32_t id);
        const Buffer* const ed_fetch_buffer_const(const int32_t id) const;
        const std::vector<int32_t>& ed_get_open(void);
        const int32_t& ed_get_current_id(void);
        const int& ed_get_error(void);
        
        Editor& ed_eval_cmd(void);
        void ed_clear_cmd(void);
        std::pair<std::string, int32_t> ed_do_cmd(void);
        void ed_cmd_ins(const unsigned char c);
        const EditorCmd& ed_get_cmd(void) const { return cmd; }
        
    private:
        int error;
        uint8_t mode;
        int32_t current_buffer_id;
        std::unordered_map<int32_t, Buffer*> bufs;
        std::unordered_set<int32_t> used_ids;
        std::vector<int32_t> open;
        EditorCmd cmd;
};

#endif