#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"

#include <iostream>
#include <unordered_set>

static const std::unordered_set<std::string> keywords = {
    "if", "else", "for", "while", "return", "switch", "case", "break", "continue"
};

static const std::unordered_set<std::string> types = {
    "int", "float", "double", "char", "void", "bool", "unsigned", "long", "short"
};

static const std::unordered_set<std::string> operators = {
    "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!"
};


const uint8_t char_type(const char c){
    if(c == SPACE_CHAR) {return SPACE_TOKEN; }
    if(c == '\t') { return TAB_TOKEN; }

    if(std::string("+-*/%&|^=<>!").find(c) != std::string::npos) {
        return OPERATOR_TOKEN;
    }

    if(std::ispunct(c)) { return PUNCT_TOKEN; }
    if(std::isalpha(c)) { return CHAR_TOKEN; }
    if(std::isdigit(c)) { return NUMERIC_TOKEN; }
    return CONTROL_TOKEN;
}

static uint8_t substr_type(const std::string& s){
    if(keywords.count(s)) { return KEYWORD; }
    if(types.count(s)) { return TYPE_DEFINITION; }
    if(operators.count(s)) { return OPERATORS; }
    return GENERIC_TEXT;
}

Buffer& Buffer::buf_tokenize(void){
    token_buffer.clear();
    const int size = buf_get_size();
    for(int i = 0; i < size; i++){
        const int strsize = buf_get_line_size(i);
        std::vector<Token> token_line;

        int j = 0;
        while(j < strsize){
            Token t = {
                buffer[i][j], char_type(buffer[i][j])
            };
            token_line.push_back(t);
            j++;
        }

        token_buffer.push_back(token_line);
    }

    return *this;
}

void Buffer::buf_semantic_group(void){
    group_buffer.clear();
    const int size = static_cast<int>(token_buffer.size());

    for(int i = 0; i < size; i++){
        std::vector<Group> groups;
        const int vecsize = token_buffer[i].size();
        
        int j = 0;
        while(j < vecsize){
            const unsigned char current = token_buffer[i][j].identifier;
            std::string substr;

            int k = j;
            while(k < vecsize && token_buffer[i][k].identifier == current){
                substr += token_buffer[i][k].token;
                k++;
            }

            Group g = {substr, substr_type(substr)};
            groups.push_back(g);

            j = k;
        }

        group_buffer.push_back(groups);
    }
}