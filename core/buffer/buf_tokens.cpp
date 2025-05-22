#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"

#include <iostream>
#include <unordered_set>
#include <functional>

static const std::unordered_set<std::string> keywords = {
    "if", "else", "for", "while", "return", "switch", "case", "break", "continue", "static", "const"
};

static const std::unordered_set<std::string> types = {
   "size_t", "int", "float", "double", "char", "void", "bool", "unsigned", "long", "short"
};

static const std::unordered_set<std::string> operators = {
    "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!"
};

static const std::unordered_set<uint8_t> delimiters = {
    OPERATOR_TOKEN, PUNCT_TOKEN, SPACE_TOKEN, TAB_TOKEN
};


const int space_char(const char c){
    return std::string(" \t").find(c) != std::string::npos;
}

const uint8_t identifier_string(const std::string& s){
    if(types.count(s)) {
        return TYPE_DEFINITION;
    } else {
        return GENERIC_TEXT;
    }
}

const int keyword_string(const std::string& s){
    if(keywords.count(s)){
        return KEYWORD;
    } else {
        return GENERIC_TEXT;
    }
}

const int numeric_char(const int c){
    return std::isdigit(c);
}

const int regular_char(const char c){
    return std::isalnum(c) || c == '_';
}

const int operator_char(const char c){
    return std::string("+-*/%&|^=<>!").find(c) != std::string::npos;
}

const int punct_char(const char c){
    return std::string("(){}[];:,.").find(c) != std::string::npos;
}

const int preprocessor_char(const char c){
    return c == '#';
}

const int quote_char(const char c){
    return c == '"' || c == '\'';
}

const int reverse_slash(const char c){
    return c == '\\';
}

struct Rule {
    std::function<const int(const char)> fn;
    uint8_t type;
};

static const std::array<Rule, 8> rules = {{
    { space_char, IGNORE },
    { numeric_char, DIGITS },
    { regular_char, GENERIC_TEXT }, 
    { operator_char, OPERATORS },
    { punct_char, PUNCTUATION },
    { preprocessor_char, GENERIC_TEXT },
    { quote_char, GENERIC_TEXT },
    { reverse_slash, GENERIC_TEXT }
}};

static void semantic_grouping(const char c, const int strsize, int& j, const int i, std::vector<std::string>& buffer, std::vector<Group>& grouping){
    const int rsize = static_cast<int>(rules.size());
    int matched = 0;

    for(int k = 0; k < rsize; k++){
        if(rules[k].fn(c)){
            std::string substr;
            while(j < strsize && rules[k].fn(buffer[i][j])){
                substr += buffer[i][j];
                j++;
            }

            uint8_t type = rules[k].type;

            if(type == GENERIC_TEXT){
                if(identifier_string(substr) != GENERIC_TEXT){
                    type = TYPE_DEFINITION;
                } else if(keyword_string(substr) != GENERIC_TEXT){
                    type = KEYWORD;
                }
            }

            grouping.push_back({substr, type});
            matched = 1;
        }
    }

    if(!matched){
        grouping.push_back({std::string(1, buffer[i][j]), GENERIC_TEXT});
        j++;
    }
}


Buffer& Buffer::buf_tokenize(void){
    group_buffer.clear();
    const int size = buf_get_size();

    for(int i = 0; i < size; i++){ 
        const int strsize = buf_get_line_size(i);
        int j = 0;

        std::vector<Group> grouping;
        while(j < strsize){
            const char c = buffer[i][j];
            semantic_grouping(c, strsize, j, i, buffer, grouping);
        }
        group_buffer.push_back(grouping);
    }

    return *this;
}

