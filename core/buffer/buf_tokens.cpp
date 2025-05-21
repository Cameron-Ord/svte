#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"

#include <iostream>

static std::string char_type(const unsigned char c){
    if(c == SPACE_CHAR) {return "space"; }
    if(c == '\t') { return "tab"; }
    if(std::ispunct(c)) { return "punct"; }
    if(std::isalpha(c)) { return "letter"; }
    if(std::isdigit(c)) { return "digit"; }
    return "control";
}

// Basic tokenization by character type - will have to also identify things by keywords for syntax highlighting
std::vector<std::vector<Token>> Buffer::buf_tokenize(void){
    std::vector<std::vector<Token>> token_buffer;
    
    const int size = buf_get_size();
    for(int i = 0; i < size; i++){
        const int strsize = buf_get_line_size(i);
        std::vector<Token> token_line;

        int j = 0;
        while(j < strsize){
            std::string substr, cur_char_type = char_type(buffer[i][j]);

            int k = j;
            while(k < strsize && char_type(buffer[i][k]) == cur_char_type){
                substr += buffer[i][k];
                k++;
            }

            token_line.push_back({substr, cur_char_type});
            j = k;
        }

        token_buffer.push_back(token_line);
    }

    return token_buffer;
}

void Buffer::buf_line_retokenize(void){

}