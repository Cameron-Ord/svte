#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>

#include "core_token.hpp"

// Maybe consider breaking this up into subclasses
class Buffer {
    public:
        Buffer(const int32_t id);
        Buffer(const int32_t id, std::string fn);
        
        void buf_set_id(const int32_t id) { buffer_id = id; }
        void buf_set_filename(const std::string fn) { filename = fn; }
        void buf_set_error(const int err) { error = err; }
        void buf_set_row(const int val) { row = val; }
        void buf_set_col(const int val) { col = val; }

        //Getters
        const int buf_get_size(void) const { return static_cast<int>(buffer.size()); }
        const std::vector<std::vector<Token>>& _buffer(void) const { return buffer; }

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
        void buf_update_col(const int updated_row);
        const int buf_valid_row(const int row) const;
        const int buf_valid_col(const int col) const;

        void buf_ins_char(const char c);
        void buf_ins_tokens(ColIt& str, std::vector<Token>& line, const std::vector<Token> sub);
        void buf_ins_row(const int next, const std::vector<Token> sub);

        void buf_erase_tokens(ColIt &tokens, std::vector<Token>& line);
        void buf_erase_token(ColIt &tokens, std::vector<Token>&line);
        void buf_rmv_before(void);
        void buf_rmv_at(void);
        void buf_rmv_line(RowIt& vec);
        void buf_retokenize(void);
        void buf_tokenize(const std::vector<std::string> readbuffer);

        int buf_mv_row(const int amount);
        void buf_mv_col(const int amount);

        std::vector<Token> buf_tokens_after_col(std::vector<Token>::const_iterator& begin, std::vector<Token>::const_iterator& end);

        ~Buffer(void);
    private:
        int error;
        int32_t buffer_id;
        int row, col, saved_col;
        std::string filename;
        std::vector<std::vector<Token>> buffer;
};

#endif