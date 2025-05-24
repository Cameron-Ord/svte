#include "../include/core/core_parser.hpp"
#include <iostream>
#include <array>

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

    charbranch = {
        { PUNCT_TOKEN, [this](ParseArg& info) { char_followed_by_punct(info); } }
    };
    
    tree = {
        {CHAR_TOKEN, charbranch}, /* {OPERATOR_TOKEN, opbranch}, {QUOTATION_TOKEN, quotebranch}, 
        {NUMERIC_TOKEN, numbranch}, {PUNCT_TOKEN, punctbranch}*/
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
    for(size_t i = 0; i < line.size(); i++){
        row.push_back({line[i], find_type(line[i])});
    }
    return row;
}

//Not done here, once they are tokenized, I want to essentially evaluate and reassign the token type
//depending on how the evaluation goes.

//There are other token types that are not applied initially since they require more context.
std::vector<std::vector<Token>> TokenParser::tokenize(void){
    std::vector<std::vector<Token>> tokenized;
    for(size_t i = 0; i < buf_ref.size(); i++){
        tokenized.push_back(build_row(buf_ref[i]));
    }

    for(size_t i = 0; i < tokenized.size(); i++){
        size_t j = 0;

        uint16_t prev_identifier = NULL_CHAR;
        while(j < tokenized[i].size()){
            const uint16_t current_identifier = tokenized[i][j].identifier;
            const size_t start = j;
            size_t k = j;

            //All we are doing here is going forward at the started position until we find a different 
            //char type and filling a substr.
            std::string substr;
            while(k < tokenized[i].size() && tokenized[i][k].identifier == current_identifier){
                substr += tokenized[i][k].token;
                k++;
            }

            if(!(k < tokenized[i].size())){
                j = k;
                continue;
            }
            
            ParseArg args = {
                current_identifier, tokenized[i][k].identifier, 
                tokenized[i],  start, k, substr
            };

            auto t = tree.find(current_identifier);
            if(t != tree.end()){
                auto b =  t->second.find(args.next);
                if(b != t->second.end()){
                    b->second(args);
                }
            }

            j = k;
        }
    }

    return tokenized;
}


void TokenParser::char_followed_by_punct(ParseArg& info){
    switch(info.tb[info.end].token){
        default: break;

        case '(':{
            //Simply walk backwards until a non alpha char is found and paint as a func
            for(size_t i = info.end - 1; i >= 0 && info.tb[i].identifier == info.current; i--){
                info.tb[i].identifier = FUNCTION_TOKEN;
            }
        }break;

        case '.':{
            // obviously I need to take into account possible chained functions and such..
            for(size_t i = info.end - 1; i >= 0 && info.tb[i].identifier == info.current; i--){
                if(info.tb[i].token == ')'){
                    break;
                }
                info.tb[i].identifier = VARIABLE_TOKEN;
            }
        }break;

        case ';':{

        }break;
    }
}   

void type_char_untiL_space(std::vector<Token>& tb, const size_t start, const size_t end){

}