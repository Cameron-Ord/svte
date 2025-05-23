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
    OOB = 10,
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
    RowIt(std::vector<std::vector<Token>>& arg);
    void offset(const int val);
};

struct RowItConst {
    std::vector<std::vector<Token>>::const_iterator begin;
    std::vector<std::vector<Token>>::const_iterator end;
    RowItConst(const std::vector<std::vector<Token>>& arg);
    void offset(const int val);
};


struct ColIt{
    std::vector<Token>::iterator begin;
    std::vector<Token>::iterator end;
    std::vector<Token> strconv;

    ColIt(std::vector<Token>& arg);
    ColIt(const std::string& arg);
    void reset(std::vector<Token> &arg);
    void offset(const int val);
};


struct ColItConst{
    std::vector<Token>::const_iterator begin;
    std::vector<Token>::const_iterator end;
    std::vector<Token> strconv;

    ColItConst(const std::vector<Token>& arg);
    ColItConst(const std::string& arg);
    void reset(const std::vector<Token> &arg);
    void offset(const int val);
};



#endif