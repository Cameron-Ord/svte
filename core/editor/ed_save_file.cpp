#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>
#include <fstream>

int Editor::ed_save_file(const int32_t id){
    std::unordered_map<int32_t, class Buffer *>::iterator it;
    it = bufs.find(id);
    if(it != bufs.end()){
        class Buffer *buffer = it->second;
        const std::string fullpath = ed_working_path + ed_delimiter() + buffer->buf_get_filename();
        
        std::ofstream file(fullpath, std::ios::out);
        if(!file.is_open()){
            std::cerr << "Failed to open file!" << std::endl;
            return BAD_OPEN;
        }
        
        size_t accumulator = 0;
        std::vector<std::string>::iterator row_iterator = buffer->buf_row_begin();
        std::vector<std::string>::iterator end = buffer->buf_row_end();
        
        for(; row_iterator != end; ++row_iterator){
            std::string str = *row_iterator;
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