#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>
#include <fstream>

int Editor::ed_open_file(const int32_t id){
    std::unordered_map<int32_t, class Buffer *>::iterator it;
    it = bufs.find(id);
    
    if(it != bufs.end()){
        class Buffer *buffer = it->second;
        
        buffer->buf_zero_buffer();
        const std::string fullpath = ed_working_path + ed_delimiter() + buffer->buf_get_filename();

        std::ifstream file(fullpath, std::ios::in);
        if(!file.is_open()){
            std::cerr << "Failed to open file!" << std::endl;
            buffer->buf_append_line("");
            return BAD_OPEN;
        }

        std::string line;
        int line_count = 0;
        int byte_accumulator = 0;

        while (std::getline(file, line)) {
            buffer->buf_append_line(line);
            byte_accumulator += line.size() * sizeof(line[0]);
            line_count++;
        }

        const int cond = buffer->buf_get_size() >= 1;
        switch(cond){
            case 1:{
                std::cout << "Opened file with " << line_count << " lines and ";
                std::cout << byte_accumulator << " bytes" << std::endl;
            } break;

            case 0:{
                std::cout << "Empty file!" << std::endl;
                buffer->buf_append_line("");
            } break;
        }

        file.close();
        return CORE_NIL;
    } 
    return NO_BUFFER;
}