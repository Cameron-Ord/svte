#include "../include/core/core_parser.hpp"
#include <iostream>


//Right now kinda just assuming the lang is C.

TokenParser::TokenParser(const std::vector<std::string>& buf) : buf_ref(buf){

    ch = {
        [this](char c) -> const uint16_t { return space_char(c); },
        [this](char c) -> const uint16_t { return quote_char(c); },
        [this](char c) -> const uint16_t { return symbol_char_start(c); },
        [this](char c) -> const uint16_t { return numeric_char(c); }, 
        [this](char c) -> const uint16_t { return operator_char(c); },
        [this](char c) -> const uint16_t { return punct_char(c); },
    };

    comment = {
        "//", "/*", "*/"
    };

    keywords = {
        "if", "else", "for", "while", "return", "switch", "case", 
        "break", "continue", "static", "const"
    };

    types = {
        "size_t", "int", "float", "double", "char", 
        "void", "bool", "unsigned", "long", "short"
    };

    operators = {
        "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", 
        ">=", "&&", "||", "!", "+=", "-=", "&"
    };


}

const uint16_t TokenParser::space_char(const char c){
    if(c == ' ' || c == '\t'){
        return SPACE_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint16_t TokenParser::numeric_char(const char c){
    if(std::isdigit(c)){
        return NUMERIC_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint16_t TokenParser::symbol_char_start(const char c){
    if(std::isalpha(c) || c == '_'){
        return CHAR_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint16_t TokenParser::symbol_char(const char c){
    if(std::isalnum(c) || c == '_'){
        return ALHPA_NUMERIC;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint16_t TokenParser::operator_char(const char c){
    if(std::string("+-*/%&|^=<>!").find(c) != std::string::npos){
        return OPERATOR_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint16_t TokenParser::punct_char(const char c){
    if(std::string("!#$,-.:;?@[]`|~(){}\\").find(c) != std::string::npos){
        return PUNCT_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint16_t TokenParser::quote_char(const char c){
    if(c == '\'' || c == '"'){
        return QUOTATION_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint16_t TokenParser::find_type(const char c){
    for(size_t i = 0; i < ch.size(); i++){
        const uint16_t type = ch[i](c);
        if(type != UNKNOWN_TOKEN){
            return type;
        }
    }
    return UNKNOWN_TOKEN;
}

std::vector<Token> TokenParser::build_row(const std::string& line){
    std::vector<Token> row;
    for(size_t i = 0; i < line.size(); i++){
        row.push_back({line[i], find_type(line[i])});
    }
    return row;
}


std::vector<std::vector<Token>> TokenParser::tokenize(void){
    std::vector<std::vector<Token>> tokenized;
    for(size_t i = 0; i < buf_ref.size(); i++){
        tokenized.push_back(build_row(buf_ref[i]));
    }
    return tokenized;
}
