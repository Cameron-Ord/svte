#include "../include/core/core_parser.hpp"
#include <iostream>


//Right now kinda just assuming the lang is C.

TokenParser::TokenParser(const std::vector<std::string>& buf) : buf_ref(buf){
    fn.insert({CHAR_TOKEN, [this](std::string::const_iterator& begin, std::string::const_iterator end) -> const Group { return char_begin(begin, end); }});
    fn.insert({NUMERIC_TOKEN, [this](std::string::const_iterator& begin, std::string::const_iterator end) -> const Group { return numeric_begin(begin, end); }});
    fn.insert({PUNCT_TOKEN, [this](std::string::const_iterator& begin, std::string::const_iterator end) -> const Group { return punct_begin(begin, end); }});
    fn.insert({OPERATOR_TOKEN, [this](std::string::const_iterator& begin, std::string::const_iterator end) -> const Group { return operator_begin(begin, end); }});
    fn.insert({QUOTATION_TOKEN, [this](std::string::const_iterator& begin, std::string::const_iterator end) -> const Group { return string_begin(begin, end); }});
    fn.insert({SPACE_TOKEN, [this](std::string::const_iterator& begin, std::string::const_iterator end) -> const Group { return space_begin(begin, end); }});

    ch = {
        [this](char c) -> const uint8_t { return space_char(c); },
        [this](char c) -> const uint8_t { return symbol_char_start(c); },
        [this](char c) -> const uint8_t { return numeric_char(c); }, 
        [this](char c) -> const uint8_t { return operator_char(c); },
        [this](char c) -> const uint8_t { return punct_char(c); },
        [this](char c) -> const uint8_t { return quote_char(c); },
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
        ">=", "&&", "||", "!"
    };


}

const uint8_t TokenParser::space_char(const char c){
    if(c == ' ' || c == '\t'){
        return SPACE_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint8_t TokenParser::numeric_char(const char c){
    if(std::isdigit(c)){
        return NUMERIC_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint8_t TokenParser::symbol_char_start(const char c){
    if(std::isalpha(c) || c == '_'){
        return CHAR_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint8_t TokenParser::symbol_char(const char c){
    if(std::isalnum(c) || c == '_'){
        return ALHPA_NUMERIC;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint8_t TokenParser::operator_char(const char c){
    if(std::string("+-*/%&|^=<>!").find(c) != std::string::npos){
        return OPERATOR_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint8_t TokenParser::punct_char(const char c){
    if(std::string("!#$,-.:;?@[]`|~\\").find(c) != std::string::npos){
        return PUNCT_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint8_t TokenParser::quote_char(const char c){
    if(c == '\'' || c == '"'){
        return QUOTATION_TOKEN;
    } else {
        return UNKNOWN_TOKEN;
    }
}

const uint8_t TokenParser::char_key(const char c){
    for(size_t i = 0; i < ch.size(); i++){
        const uint8_t key = ch[i](c);
        if(key != UNKNOWN_TOKEN){
            return key;
        }
    }
    return UNKNOWN_TOKEN;
}


std::vector<std::vector<Group>> TokenParser::tokenize_by_group(void){
    const int ssize = (int)buf_ref.size();
    std::vector<std::vector<Group>> groups;

    for(int i = 0; i < ssize; i++){
        std::vector<Group> grouping;
        
        std::string::const_iterator str_begin = buf_ref[i].begin();
        std::string::const_iterator str_end = buf_ref[i].end();

        while(str_begin != str_end && !(str_begin > str_end)){
            auto it = fn.find(char_key(*str_begin));
            if(it != fn.end()){
                auto tmp = str_begin;
                const Group g = it->second(tmp, buf_ref[i].end());
                grouping.push_back(g);
                str_begin = tmp;
            } else {
                grouping.push_back({std::string(1, *str_begin), GENERIC_TEXT});
                ++str_begin;
            }
        }

        groups.push_back(grouping);
    }

    return groups;
}

const Group TokenParser::space_begin(std::string::const_iterator& begin, std::string::const_iterator end){
    std::string substr;

    for(; begin != end && space_char(*begin) != UNKNOWN_TOKEN; ++begin){
        substr += *begin;
    }

    return {substr, IGNORE};
}


const Group TokenParser::char_begin(std::string::const_iterator& begin, std::string::const_iterator end){
    std::string substr;

    for(; begin != end && symbol_char(*begin) != UNKNOWN_TOKEN; ++begin){
        substr += *begin;
    }

    if(keywords.count(substr)){
        return {substr, KEYWORD};
    } else if (types.count(substr)){
        return {substr, TYPE_DEFINITION};
    } else {
        return {substr, GENERIC_TEXT};
    }
}

const Group TokenParser::numeric_begin(std::string::const_iterator& begin, std::string::const_iterator end){
    std::string substr;

    for(; begin != end && numeric_char(*begin) != UNKNOWN_TOKEN; ++begin){
        substr += *begin;
    }

    return {substr, DIGITS};
}

const Group TokenParser::punct_begin(std::string::const_iterator& begin, std::string::const_iterator end){
    std::string substr;

    for(; begin != end && punct_char(*begin) != UNKNOWN_TOKEN; ++begin){
        substr += *begin;
    }

    if(comment.count(substr)){
        for(; begin != end && *begin; ++begin){
            substr += *begin;
        }
        return {substr, COMMENT};
    } else {
        return {substr, PUNCTUATION};
    }

}

const Group TokenParser::operator_begin(std::string::const_iterator& begin, std::string::const_iterator end){
    std::string substr;
    
    for(; begin != end && operator_char(*begin) != UNKNOWN_TOKEN; ++begin){
        substr += *begin;
    }

    if(comment.count(substr)){
        for(; begin != end && *begin; ++begin){
            substr += *begin;
        }
        return {substr, COMMENT};
    } else if (operators.count(substr)){
        return {substr, OPERATORS};
    } else {
        return {substr, GENERIC_TEXT};
    }
}

const Group TokenParser::string_begin(std::string::const_iterator& begin, std::string::const_iterator end){
    std::string substr;
    int s = 0;
    for(; begin != end; ++begin){
        if(s > 1){
            return {substr, STRING};
        }

        if(quote_char(*begin) != UNKNOWN_TOKEN){
            s++;
        }
        
        substr += *begin;
    }


    return {substr, STRING};
}