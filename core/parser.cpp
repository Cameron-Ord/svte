#include "../include/core/core_parser.hpp"
#include <iostream>
#include <array>

//Right now kinda just assuming the lang is C.

TokenParser::TokenParser(const std::vector<std::string>& buf) : buf_ref(buf){

    ch = {
        [this](char c) -> const uint16_t { return space_char(c); },
        [this](char c) -> const uint16_t { return quote_char(c); },
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
        return ALPHA_NUMERIC;
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

//std::array<std::unordered_map<uint16_t, std::function<void(std::vector<Token>&, const size_t, const size_t)>>, 5> branches = {
//    charbranch = {{PUNCT_TOKEN, char_followed_by_punct}}, opbranch, quotebranch, punctbranch, numbranch
//};

//std::unordered_map<uint16_t, std::unordered_map<uint16_t, std::function<void(std::vector<Token>&, const size_t, const size_t)>>> tree = {
//    {CHAR_TOKEN, charbranch}, {OPERATOR_TOKEN, opbranch}, {QUOTATION_TOKEN, quotebranch}, 
//    {NUMERIC_TOKEN, numbranch}, {PUNCT_TOKEN, punctbranch}
//};



std::vector<Token> TokenParser::build_row(const std::string& line){
    std::vector<Token> row;
    size_t k = 0;
    while(k < line.size()){
        if(symbol_char_start(line[k]) != UNKNOWN_TOKEN){
            row.push_back({line[k], CHAR_TOKEN});

            size_t j = k + 1;
            while(j < line.size() && symbol_char(line[j]) != UNKNOWN_TOKEN){
                row.push_back({line[j], CHAR_TOKEN}); 
                j++;
            }
            
            k = j;
            continue;
        }


        row.push_back({line[k], find_type(line[k])});
        k++;
    }
    return row;
}

//Not done here, once they are tokenized, I want to essentially evaluate and reassign the token type
//depending on how the evaluation goes.

//There are other token types that are not applied initially since they require more context.

//Also this just assumes everything is one line.. gonna have to add logic for multiline checks too..
std::vector<std::vector<Token>> TokenParser::tokenize(void){
    std::vector<std::vector<Token>> tokenized;
    for(size_t i = 0; i < buf_ref.size(); i++){
        tokenized.push_back(build_row(buf_ref[i]));
    }

    for(size_t i = 0; i < tokenized.size(); i++){
        uint16_t prev_identifier = NULL_CHAR;
        const int size = tokenized[i].size();
        int j = 0;

        while(j < size){
            const uint16_t current_identifier = tokenized[i][j].identifier;
            const int start = j;
            int k = j;
            //All we are doing here is going forward at the started position until we find a different 
            //char type and filling a substr.
            std::string substr;
            while(k < size && tokenized[i][k].identifier == current_identifier){
                substr += tokenized[i][k].token;
                k++;
            }

            j = k;
            const uint16_t next_identifier = k >= size ? current_identifier : tokenized[i][k].identifier;

            ParseArg args = {
                current_identifier, next_identifier, 
                tokenized[i],  start, k, substr
            };

            switch(args.current){
                case CHAR_TOKEN:{
                    scan_set(args);
                    switch(args.next){
                        case PUNCT_TOKEN:{
                            char_before_punct(args);
                        }break;  
                    }
                }break;

                case QUOTATION_TOKEN:{
                    int broken_escape = 0;
                    int d = start + 1; 
                    while(d < size && quote_char(args.tb[d].token) != QUOTATION_TOKEN){
                        if(args.tb[d].token == '\\'){
                            int s = 0;
                            while(s < size && quote_char(args.tb[d].token) != QUOTATION_TOKEN){
                                if(args.tb[s].token != '\\'){
                                    args.tb[s].identifier = QUOTATION_TOKEN;
                                }
                                s++;
                            }
                            d = s;
                        }
                        
                        if(d < size && args.tb[d].token != '\\'){
                            args.tb[d].identifier = QUOTATION_TOKEN;
                        }
                        d++;
                    }
                }break;
            }
        }
    }

    return tokenized;
}

void TokenParser::scan_set(ParseArg& info){
    if(types.count(info.substr)){
        for(int d = info.end - 1; d >= info.start && d >= 0; d--){
            info.tb[d].identifier = TYPE_TOKEN;
        }
    } else if (keywords.count(info.substr)){
        for(int d = info.end - 1; d >= info.start && d >= 0; d--){
            info.tb[d].identifier = KEYWORD_TOKEN;
        }
    } else if (comment.count(info.substr)){
        for(int d = info.end - 1; d >= info.start && d >= 0; d--){
            info.tb[d].identifier = COMMENT_TOKEN;
        }
    }
}

void TokenParser::char_before_punct(ParseArg& info){
    switch(info.tb[info.end].token){
        default: break;

        case '(':{
            //Simply walk backwards until a non alpha char is found and paint as a func
            for(int i = info.end - 1; i >= 0 && info.tb[i].identifier == info.current; i--){
                info.tb[i].identifier = FUNCTION_TOKEN;
            }
        }break;

        case '.':{
            // obviously I need to take into account possible chained functions and such..
            for(int i = info.end - 1; i >= 0 && info.tb[i].identifier == info.current; i--){
                if(info.tb[i].token == ')'){
                    break;
                }
                info.tb[i].identifier = VARIABLE_TOKEN;
            }
        }break;

        case ';':{

        }break;

        case ')': {
        }break;

        case '}': {

        }break;

        case ']' : {

        }break;
    }
}   

void type_char_untiL_space(std::vector<Token>& tb, const size_t start, const size_t end){

}