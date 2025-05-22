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

class Editor {
    public:
        Editor(void);
        ~Editor(void);
        int32_t ed_next_id(void);
        int32_t ed_prev_id(void);
        int32_t ed_gen_id(void);

        void ed_cmd_ins(const char *c, const size_t size);
        void ed_ins_char(const int32_t id, const char *c, const size_t size);

        void ed_set_cmd_mode(const int val) { cmd_mode = val; }
        const int ed_taking_cmd(void) const { return cmd_mode; }

        void ed_set_current_id(const int32_t id);
        int ed_append_buffer(std::pair<std::string, std::vector<std::string>> commit);
        void ed_set_error(const int err);

        Buffer *ed_fetch_buffer(const int32_t id);
        const Buffer* const ed_fetch_buffer_const(const int32_t id) const;
        const std::vector<int32_t>& ed_get_open(void);
        const int32_t& ed_get_current_id(void);
        const int& ed_get_error(void);
        void ed_clear_cmd(void) {
            cmd.cmdstr.clear();
            cmd.cursor = 0;
        }
        
        int ed_eval_cmd(void);
        const EditorCmd& ed_get_cmd(void) const { return cmd; }
        
    private:
        int error;
        int32_t current_buffer_id;
        //0 - 1
        int cmd_mode;
        std::unordered_map<int32_t, Buffer*> bufs;
        std::unordered_set<int32_t> used_ids;
        std::vector<int32_t> open;
        EditorCmd cmd;
};

#endif
