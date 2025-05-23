#ifndef CORE_TOKEN_HPP
#define CORE_TOKEN_HPP

#include <string>
#include <cstdint>
#include <vector>

#include "core_defines.hpp"

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