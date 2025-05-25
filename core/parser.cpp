#include "../include/core/core_parser.hpp"
#include <iostream>
#include <array>

//Right now kinda just assuming the lang is C/C++. Obviously i need to make things variables and change things slightly based on the loaded file type.
//Since other languages dont follow each others conventions 1:1 

const char ROUND_PARAN_LEFT = '(';
const char ROUND_PARAN_RIGHT = ')';
const char CURL_PARAN_LEFT = '{';
const char CURL_PARAN_RIGHT = '}';
const char SQUARE_PARAN_LEFT = '[';
const char SQUARE_PARAN_RIGHT = ']';
const char MEMBER_ACCESS = '.';
const char ESCAPE_CHAR = '\\';
const char SEMICOLON = ';';
const char FORWARD_BRACKET = '/';

static const char next_char(const int start, const int end, const std::vector<Token>& t){
    for(int d = start; d < end; d++){
        if(t[d].token != SPACE_CHAR){
            return t[d].token;
        }
    }
    return 0;
}

static const char prev_char(const int start, const std::vector<Token>& t){
    for(int d = start - 1; d >= 0; d--){
        if(t[d].token != SPACE_CHAR){
            return t[d].token;
        }
    }
    return 0;
}


static void set_next_char_type(const int start, const int end, const int type_id, std::vector<Token>& t){
    for(int d = start; d < end; d++){
        if(t[d].token != SPACE_CHAR){
            t[d].identifier = type_id;
            return;
        }
    }
}

static void set_prev_char_type(const int start, const int type_id, std::vector<Token>& t){
    for(int d = start - 1; d >= 0; d--){
        if(t[d].token != SPACE_CHAR){
            t[d].identifier = type_id;
            return;
        }
    }
}

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
        ">=", "&&", "||", "!", "+=", "-=", "&", "->"
    };


}

TokenParser::TokenParser(void) : buf_ref(std::vector<std::string>({""})){
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
        ">=", "&&", "||", "!", "+=", "-=", "&", "->"
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

//This does some basic lexing per line, 
//but at the moment it does not take in to account multiline stuff 
//for things like quote containing strings

void TokenParser::lex(std::vector<std::vector<Token>>& tb){
    for(size_t i = 0; i < tb.size(); i++){
        int j = 0;
        const int size = tb[i].size();
        while(j < size){
            const uint16_t current_identifier = tb[i][j].identifier;
            std::string substr;
            const int start = j;
            int end = start;

            for(int k = j; k < size && tb[i][k].identifier == current_identifier; k++){
                substr += tb[i][k].token;
                end++;
            }

            switch(current_identifier){
                default: break;

                case OPERATOR_TOKEN:{
                    if(comment.count(substr)){
                        for(int d = start; d < size; d++){
                            tb[i][d].identifier = COMMENT_TOKEN;
                        }
                    }
                };

                case PUNCT_TOKEN:{
                }break;

                case NUMERIC_TOKEN:{
                }break;

                case CHAR_TOKEN:{
                    if(partial_set_match(substr, types)){
                        for(int d = start; d < end; d++){
                            tb[i][d].identifier = TYPE_TOKEN;   
                        }
                    } else if (partial_set_match(substr, keywords)){
                        for(int d = start; d < end; d++){
                            tb[i][d].identifier = KEYWORD_TOKEN;   
                        }
                    }

                    if(next_char(end, size, tb[i]) == '>' && prev_char(start, tb[i]) == '<'){
                        set_prev_char_type(start, CHAR_TOKEN, tb[i]);
                        set_next_char_type(end, size, CHAR_TOKEN, tb[i]);
                    }

                    switch(next_char(end, size, tb[i])){
                        case ROUND_PARAN_LEFT:{
                            for(int d = end - 1; d >= start && d >= 0; d--){
                                tb[i][d].identifier = FUNCTION_TOKEN;
                            }
                        }break;

                        case MEMBER_ACCESS: {
                            for(int d = end - 1; d >= start && d >= 0; d--){
                                tb[i][d].identifier = VARIABLE_TOKEN;
                            }
                        }break;
                    }
                }break;

                case QUOTATION_TOKEN:{
                    int d = start + 1; 
                    while(d < size && quote_char(tb[i][d].identifier) != QUOTATION_TOKEN){
                   
                        if(tb[i][d].token == ESCAPE_CHAR){
                            int s = 0;
                            while(s < size && quote_char(tb[i][d].token) != QUOTATION_TOKEN){
                                if(tb[i][d].token != ESCAPE_CHAR){
                                    tb[i][d].identifier = QUOTATION_TOKEN;
                                }
                                s++;
                            }
                            d = s;
                        }
                        
                        if(d < size && tb[i][d].token != ESCAPE_CHAR){
                            tb[i][d].identifier = QUOTATION_TOKEN;
                        }
                        d++;
                    }
                }break;
            }

            j += substr.size();
        }
    }
}

std::vector<std::vector<Token>> TokenParser::tokenize(void){
    std::vector<std::vector<Token>> tokenized;
    for(size_t i = 0; i < buf_ref.size(); i++){
        tokenized.push_back(build_row(buf_ref[i]));
    }
    lex(tokenized);
    return tokenized;
}


void TokenParser::retokenize(std::vector<std::vector<Token>>& tb){
    lex(tb);
}

int TokenParser::partial_set_match(const std::string& substr, const std::unordered_set<std::string>& set){
    for(const auto& val : set){
        if(substr.size() >= val.size() && substr.compare(0, val.size(), val) == 0){
            return 1;
        }
    }
    return 0;
}
