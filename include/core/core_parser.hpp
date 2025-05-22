#ifndef CORE_PARSER_HPP
#define CORE_PARSER_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#include "../core/core_token.hpp"

struct TokenParser {
    std::unordered_set<std::string> keywords;
    std::unordered_set<std::string> types;
    std::unordered_set<std::string> operators;
    std::unordered_set<std::string> comment;


    std::unordered_map<uint8_t, std::function<const Group(std::string::const_iterator& begin, std::string::const_iterator end)>> fn;

    const std::vector<std::string>& buf_ref;
    TokenParser(const std::vector<std::string>& buf);
    std::vector<std::vector<Group>> tokenize_by_group(void);

    //const int starts_with_comment(const std::string& substr);
    //const int matches_known_identifier(const std::string &substr);
    //const int matches_known_keyword(const std::string &substr);

    const uint8_t numeric_char(const char c);
    const uint8_t symbol_char_start(const char c);
    const uint8_t symbol_char(const char c);
    const uint8_t operator_char(const char c);
    const uint8_t punct_char(const char c);
    const uint8_t quote_char(const char c);
    const uint8_t space_char(const char c);


    std::vector<std::function<const uint8_t(const char)>> ch;
    const uint8_t char_key(const char c);

    const Group char_begin(std::string::const_iterator& begin, std::string::const_iterator end);
    const Group numeric_begin(std::string::const_iterator& begin, std::string::const_iterator end);
    const Group punct_begin(std::string::const_iterator& begin, std::string::const_iterator end);
    const Group operator_begin(std::string::const_iterator& begin, std::string::const_iterator end);
    const Group string_begin(std::string::const_iterator& begin, std::string::const_iterator end);
    const Group space_begin(std::string::const_iterator& begin, std::string::const_iterator end);

};


#endif