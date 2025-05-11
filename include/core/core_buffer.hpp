#ifndef BUFFER_HPP
#define BUFFER_HPP

#define SPACECHAR ' '
#define NULLCHAR '\0'
#define NEWLINE '\n'

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
        const int& buf_get_row(void);
        const int& buf_get_col(void);
        void buf_zero_line(std::string& str);
        void buf_zero_buffer(void);

        ~Buffer(void);
    private:
        int error;
        uint32_t buffer_id;
        int row, col;
        std::string filename;
        std::vector<std::string> buffer;
};

#endif