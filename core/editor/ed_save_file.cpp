#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <fstream>
#include <iostream>

int Editor::ed_save_file(const int32_t id)
{
    std::unordered_map<int32_t, Buffer *>::iterator it;
    it = bufs.find(id);
    if (it != bufs.end()) {
        Buffer *buffer = it->second;
        const std::string fullpath = ed_working_path + ed_delimiter() + buffer->buf_get_filename();

        std::ofstream file(fullpath, std::ios::out);
        if (!file.is_open()) {
            std::cerr << "Failed to open file!" << std::endl;
            return BAD_OPEN;
        }

        size_t accumulator = 0;

        ConstBufRowIt buf(buffer->buf_get_buffer());

        for (; buf.begin != buf.end; ++buf.begin) {
            std::string str = *buf.begin;
            accumulator += str.size() * sizeof(str[0]);
            //Just needs a newline, so not using std::endl;
            file << str << '\n';
        }

        std::cout << accumulator << " Bytes written" << std::endl;
        file.close();
        return CORE_NIL;
    }
    return NO_BUFFER;
}