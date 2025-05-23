#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cstdint>


struct EditorCmd{
    std::string cmd_prefix;
    std::string cmd_arg;

    int cmd_input_mode;
    int pfx_cursor;
    int arg_cursor;

    int CMD_VALID;
    int CMD_INVALID;

    EditorCmd(void) {
        cmd_input_mode = 0;
        arg_cursor = 0;
        pfx_cursor = 0;

        CMD_VALID = 1;
        CMD_INVALID = 0;
    }
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

        const int ed_get_cmd_input_mode(void) const { return cmd.cmd_input_mode; }
        void ed_set_prefix_cmd_mode(void) { cmd.cmd_input_mode = 0; }
        void ed_set_arg_cmd_mode(void) { cmd.cmd_input_mode = 1; }

        void ed_ins_char(const int32_t id, const char *c, const size_t size);
        Editor& ed_ins_cmd(const char *c, const size_t size);

        void ed_set_cmd_mode(const int val) { cmd_mode = val; }
        const int ed_taking_cmd(void) const { return cmd_mode; }

        void ed_set_current_id(const int32_t id) { current_buffer_id = id; }
        int ed_append_buffer(std::pair<std::string, std::vector<std::string>> commit);
        void ed_set_error(const int err) { error = err; }

        Buffer *ed_fetch_buffer(const int32_t id);
        const Buffer* const ed_fetch_buffer_const(const int32_t id) const;
        const std::vector<int32_t>& ed_get_open(void) { return open; }
        const int32_t& ed_get_current_id(void) { return current_buffer_id; }
        const int& ed_get_error(void) { return error; }
        void ed_clear_cmd(void) {
            cmd.cmd_prefix.clear();
            cmd.cmd_arg.clear();
            cmd.arg_cursor = 0;
            cmd.pfx_cursor = 0;
            ed_set_prefix_cmd_mode();
        }
        
        void ed_eval_cmd(void);
        int ed_cmd_commit(void);
        int ed_cmd_pfx_to_int(void);

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
