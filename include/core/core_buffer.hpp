#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>

#include "core_iterator.hpp"

struct CursSnapshot {
    int row, col;
    CursSnapshot(const int row, const int col) : row(row), col(col){}
};

// Maybe consider breaking this up into subclasses
class Buffer {
    public:
        Buffer(const int32_t id);
        Buffer(const int32_t id, std::string fn);
        //Setters
        void buf_set_id(const int32_t id) { buffer_id = id; }
        void buf_set_filename(const std::string fn) { filename = fn; }
        void buf_set_error(const int err) { error = err; }
        void buf_set_row(const int val) { row = val; }
        void buf_set_col(const int val) { col = val; }
        void buf_select_start(void) { 
            sel_start.row = row, sel_start.col = col;
            sel_end.row = row, sel_end.col = col;
        }

        //Getters
        const int buf_get_size(void) const { return static_cast<int>(buffer.size()); }
        const std::vector<std::string>* const buf_get_buffer(void) const { return &buffer; }
        const int buf_get_line_size(const int access) const {
            if(buf_valid_row(access)){
                return static_cast<int>(buffer[access].size());
            }
            return -1;
        }
        const int& buf_get_row(void) const { return row; }
        const int& buf_get_col(void) const { return col; }
        const int& buf_get_error(void) { return error; }
        const std::string& buf_get_filename(void) const { return filename; }
        const int32_t& buf_get_id(void) const { return buffer_id; }
        void buf_select_end(void) { sel_end.row = row, sel_end.col = col; }
        const CursSnapshot& buf_get_select_start(void) const { return sel_start; }
        const CursSnapshot& buf_get_select_end(void) const { return sel_end; }

        void buf_new_line(void);   
        void buf_zero_line(std::string& str);
        void buf_zero_buffer(void);     
        void buf_append_line(const std::string line);
        void buf_update_col(const int updated_row);
        const int buf_valid_row(const int row) const;
        const int buf_valid_col(const int col) const;

        void buf_ins_char(const unsigned char c);
        void buf_ins_substr(BufStrIt& str, const std::string substr);
        void buf_ins_row(const int next, const std::string substr);

        void buf_erase_substr(BufStrIt& str);
        void buf_erase_char(BufStrIt& str);
        void buf_rmv_before(void);
        void buf_rmv_at(void);
        void buf_rmv_line(BufRowIt& vec);

        int buf_mv_row(const int amount);
        void buf_mv_col(const int amount);

        std::string buf_get_substr_after_col_pos(ConstBufStrIt& str);
        std::string buf_get_substr_before_col_pos(ConstBufStrIt& str);

        ~Buffer(void);
    private:
        int error;
        int32_t buffer_id;
        int row, col, saved_col;
        std::string filename;
        std::vector<std::string> buffer;
        CursSnapshot sel_start, sel_end;
};

#endif