#ifndef CORE_PARSER_HPP
#define CORE_PARSER_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#include "../core/core_token.hpp"

struct ParseArg {
    const uint16_t current, next;
    std::vector<Token>& tb;
    const int start, end;
    const std::string& substr;
};

struct TokenParser {
    std::unordered_set<std::string> keywords;
    std::unordered_set<std::string> types;
    std::unordered_set<std::string> operators;
    std::unordered_set<std::string> comment;
    const std::vector<std::string>& buf_ref;
  
    TokenParser(const std::vector<std::string>& buf);
    TokenParser(void);
    std::vector<std::vector<Token>> tokenize(void);
    void retokenize(std::vector<std::vector<Token>>& tb);
    void lex(std::vector<std::vector<Token>>& tb);
    void token_update(std::vector<std::vector<Token>>& tb);
    static const uint16_t numeric_char(const char c);
    static const uint16_t symbol_char_start(const char c);
    static const uint16_t symbol_char(const char c);
    static const uint16_t operator_char(const char c);
    static const uint16_t punct_char(const char c);
    static const uint16_t quote_char(const char c);
    static const uint16_t space_char(const char c);
    static const uint16_t find_type(const char c);

    std::vector<Token> build_row(const std::string& line);
    int partial_set_match(const std::string& substr, const std::unordered_set<std::string>& set);
};



#endif