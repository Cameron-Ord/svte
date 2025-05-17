#ifndef EDITOR_HPP
#define EDITOR_HPP
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cstdint>

#include "core_structs.hpp"

//Forward declare the buffer class, must be defined in SRC files.
class Buffer;

typedef enum { VISUAL = 0, INSERT = 1, CMD = 2, NVISUAL = 3, APPEND = 4 } MODES;

class Editor {
    public:
        Editor(void);
        ~Editor(void);
        
        int32_t ed_gen_id(void);
        int ed_no_file(const int32_t id);
        int ed_open_file(const int32_t id);
        int ed_save_file(const int32_t id);
        void ed_set_current_id(const int32_t id);
        void ed_mv_cursor_row(const int id, const int amount);
        void ed_mv_cursor_col(const int id, const int amount);
        int ed_append_buffer(std::string fn);
        int ed_append_buffer(void);
        void ed_set_error(const int err);
        int ed_determine_cwd(void);
        void ed_set_cwd(std::string str);
        std::string ed_fs_cwd_string(void);
        std::string ed_delimiter(void);
        class Buffer *ed_fetch_buffer(const int32_t id);
        void ed_ins_char(const int id, const unsigned char c);
        uint8_t ed_get_mode(void);
        void ed_set_mode(const uint8_t val);

        const int ed_get_col(const int id);
        const int ed_get_row(const int id);
        const class Buffer *ed_fetch_buffer_const(const int32_t id);
        const std::string& ed_get_cwd(void);
        const std::vector<int32_t>& ed_get_open(void);
        const int32_t& ed_get_current_id(void);
        const int& ed_get_error(void);
        BCommit ed_commit_buffer(std::string fn);

    private:
        int error;
        uint8_t mode;
        uint8_t is_editing;
        // current buffer ID is a signed type, for obvious purposes.
        // Othwerwise ID is ALWAYS unsigned when saved.
        int32_t current_buffer_id;
        std::string ed_working_path;
        std::unordered_map<int32_t, class Buffer*> bufs;
        std::unordered_set<int32_t> used_ids;
        std::vector<int32_t> open;
        std::vector<int32_t> shown;
};

#endif