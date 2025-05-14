#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <vector>
#include <string>

class Buffer {
    public:
        Buffer(const int32_t id);
        Buffer(const int32_t id, std::string fn);
        void buf_set_id(const int32_t id);
        const int32_t& buf_get_id(void);
        void buf_set_filename(const std::string fn);
        const std::string& buf_get_filename(void);
        void buf_set_error(const int err);
        const int& buf_get_error(void);
        void buf_set_row(const int val);
        void buf_set_col(const int val);
        const int buf_get_row(void);
        const int buf_get_col(void);
        void buf_zero_line(std::string& str);
        void buf_zero_buffer(void);
        const int buf_index_of_first_char(std::string::const_iterator it, std::string::const_iterator end);
        void buf_append_line(const std::string line);
        int buf_mv_row(const int amount);
        void buf_mv_col(const int amount);
        void buf_update_col(const int updated_row);
        const int buf_get_line_size(const int access);
        const int buf_valid_row(const int row);
        const int buf_valid_col(const int col);
        std::vector<std::string>::iterator buf_row_begin(void);
        std::vector<std::string>::iterator buf_row_end(void);
        std::vector<std::string>::const_iterator buf_row_begin_const(void);
        std::vector<std::string>::const_iterator buf_row_end_const(void);
        std::string::const_iterator buf_str_it_begin(const std::string& str);
        std::string::const_iterator buf_str_it_end(const std::string& str);
        const int buf_get_size(void);
        const std::vector<std::string>* const buf_get_buffer(void);
        void buf_ins_char(const unsigned char c);

        ~Buffer(void);
    private:
        int error;
        int32_t buffer_id;
        int row, col;
        std::string filename;
        std::vector<std::string> buffer;
};

#endif