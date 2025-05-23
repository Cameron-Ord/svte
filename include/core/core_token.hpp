#ifndef CORE_TOKEN_HPP
#define CORE_TOKEN_HPP

#include <string>
#include <cstdint>

struct Token {
    char token;
    uint8_t identifier;
};

struct Group {
    std::string str;
    uint8_t identifier;
};

typedef enum {
    CHAR_TOKEN = 0,
    OPERATOR_TOKEN = 1,
    TAB_TOKEN = 2, 
    PUNCT_TOKEN = 3, 
    NUMERIC_TOKEN = 4,
    SPACE_TOKEN = 5,
    CONTROL_TOKEN = 6,
    QUOTATION_TOKEN = 7,
    REVERSE_SLASH_TOKEN = 8,
    UNKNOWN_TOKEN = 9,
    ALHPA_NUMERIC = 10,
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

#endif