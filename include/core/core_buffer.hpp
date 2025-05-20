#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>

#include "core_iterator.hpp"

struct Selection {
    int row_start, col_start;
    int row_end, col_end;

    BufStrIt *first;
    BufStrIt *last;

    std::vector<BufStrIt *> region;

    Selection(void) : 
    row_start(0), col_start(0), row_end(0), col_end(0), first(nullptr), last(nullptr) {}

    Selection& set_row_start(const int start) { 
        row_start = start; 
        return *this;
    }

    Selection& set_row_end(const int end) { 
        row_end = end; 
        return *this;
    }

    Selection& set_col_start(const int start) { 
        col_start = start; 
        return *this;
    }

    Selection& set_col_end(const int end) { 
        row_start = end; 
        return *this;
    }

    void clear(void){ region.clear(); }

    void push_line(BufStrIt *line){
        region.push_back(line);
    }

    void insert_start(void){
        region.insert(region.begin(), first);
    }

    void insert_end(void){
        region.insert(region.end(), last);
    }
};

// Maybe consider breaking this up into subclasses
class Buffer {
    public:
        Buffer(const int32_t id);
        Buffer(const int32_t id, std::string fn);
        //Setters

        Buffer& buf_copy(std::vector<std::string> commit_vector) { 
            buffer = commit_vector; 
            return *this;
        }
        void buf_set_id(const int32_t id) { buffer_id = id; }
        void buf_set_filename(const std::string fn) { filename = fn; }
        void buf_set_error(const int err) { error = err; }
        void buf_set_row(const int val) { row = val; }
        void buf_set_col(const int val) { col = val; }

        //Getters
        const int buf_get_size(void) const { return static_cast<int>(buffer.size()); }
        const std::vector<std::string>& buf_get_buffer(void) const { return buffer; }
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
        Selection sel;
};

#endif