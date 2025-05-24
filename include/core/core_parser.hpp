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
    const size_t start, end;
    const std::string& substr;
};


struct TokenParser {
    std::unordered_set<std::string> keywords;
    std::unordered_set<std::string> types;
    std::unordered_set<std::string> operators;
    std::unordered_set<std::string> comment;

    std::unordered_map<uint16_t, std::function<void(ParseArg&)>> charbranch;
    std::unordered_map<uint16_t, std::function<void(ParseArg&)>> opbranch;
    std::unordered_map<uint16_t, std::function<void(ParseArg&)>> quotebranch;
    std::unordered_map<uint16_t, std::function<void(ParseArg&)>> punctbranch;
    std::unordered_map<uint16_t, std::function<void(ParseArg&)>> numbranch;
    std::array<std::unordered_map<uint16_t, std::function<void(ParseArg&)>>, 5> branches;
    std::unordered_map<uint16_t, std::unordered_map<uint16_t, std::function<void(ParseArg&)>>> tree;
    const std::vector<std::string>& buf_ref;
  
    TokenParser(const std::vector<std::string>& buf);
    std::vector<std::vector<Token>> tokenize(void);

    //const int starts_with_comment(const std::string& substr);
    //const int matches_known_identifier(const std::string &substr);
    //const int matches_known_keyword(const std::string &substr);

    const uint16_t numeric_char(const char c);
    const uint16_t symbol_char_start(const char c);
    const uint16_t symbol_char(const char c);
    const uint16_t operator_char(const char c);
    const uint16_t punct_char(const char c);
    const uint16_t quote_char(const char c);
    const uint16_t space_char(const char c);

    const uint16_t find_type(const char c);
    std::vector<Token> build_row(const std::string& line);

    void char_followed_by_punct(ParseArg& info);

    std::vector<std::function<const uint16_t(const char)>> ch;
};


#endif