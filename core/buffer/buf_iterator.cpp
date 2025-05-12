#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"


std::vector<std::string>::iterator Buffer::buf_get_it_at_row(std::vector<std::string>::iterator it, std::vector<std::string>::iterator end){
    for(int i = 0; it != end; ++it, ++i){
        if(i == row){
            return it;
        }
    }
    return end;
}
