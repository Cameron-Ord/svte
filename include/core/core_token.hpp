#ifndef CORE_TOKEN_HPP
#define CORE_TOKEN_HPP

#include <string>
#include <cstdint>
#include <vector>

typedef enum {
    CHAR_TOKEN = 0,
    OPERATOR_TOKEN = 1,
    PUNCT_TOKEN = 3, 
    NUMERIC_TOKEN = 4,
    SPACE_TOKEN = 5,
    QUOTATION_TOKEN = 7,
    UNKNOWN_TOKEN = 8,
    ALHPA_NUMERIC = 9, 
}TOKEN_TYPES;

typedef enum {
    GENERIC_TEXT = 0,
    OPERATORS = 1, 
    PUNCTUATION = 2,
    DIGITS = 3,
    TYPE_DEFINITION = 4,
    KEYWORD = 5,
    FUNCTION = 6,
    IGNORE = 7,
    COMMENT = 8, 
    STRING = 9
}SUBSTR_TYPES;

struct Token {
    char token;
    uint16_t identifier;
};

struct Group {
    std::string str;
    uint16_t identifier;
};


struct RowIt {
    std::vector<std::vector<Token>>::iterator begin;
    std::vector<std::vector<Token>>::iterator end;
    std::vector<std::vector<Token>>::const_iterator cbegin;
    std::vector<std::vector<Token>>::const_iterator cend;
    
    RowIt(std::vector<std::vector<Token>>& arg){
        begin = arg.begin(), end = arg.end();
    }

    RowIt(const std::vector<std::vector<Token>>& arg){
        cbegin = arg.begin(), cend = arg.end();
    }

    void offset(const int val) {
        begin = begin + val >= end ? end : begin + val;
        cbegin = cbegin + val >= cend ? cend : cbegin + val;
    }
};


struct ColIt{
    std::vector<Token>::iterator begin;
    std::vector<Token>::iterator end;
    std::vector<Token>::const_iterator cbegin;
    std::vector<Token>::const_iterator cend;

    ColIt(std::vector<Token>& arg){
        begin = arg.begin(), end = arg.end();
    }

    ColIt(const std::vector<Token>& arg){
        cbegin = arg.begin(), cend = arg.end();
    }

    void reset(const std::vector<Token> &arg){
        cbegin = arg.begin(), cend = arg.end();
    }
    
    void reset(std::vector<Token> &arg){
        begin = arg.begin(), end = arg.end();
    }
    
    std::vector<Token> strconv;
    ColIt(const std::string arg){
        for(size_t i = 0; i < arg.size(); i++){
            if(arg[i] == ' '){
                strconv.push_back({arg[i], SPACE_TOKEN});
            } else {
                strconv.push_back({arg[i], CHAR_TOKEN});
            }
        }

        cbegin = strconv.begin(), cend = strconv.end();
    }
    
    void offset(const int val) {
        begin = begin + val >= end ? end : begin + val;
        cbegin = cbegin + val >= cend ? cend : cbegin + val;
    }
};


#endif