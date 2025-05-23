
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_token.hpp"


// MUTABLE ITERATORS // 

RowIt::RowIt(std::vector<std::vector<Token>>& arg){
    begin = arg.begin(), end = arg.end();
}

void RowIt::offset(const int val) {
    begin = begin + val >= end ? end : begin + val;
}

ColIt::ColIt(std::vector<Token>& arg){
    begin = arg.begin(), end = arg.end();
}

ColIt::ColIt(const std::string& arg){
    for(size_t i = 0; i < arg.size(); i++){
        if(arg[i] == ' '){
            strconv.push_back({arg[i], SPACE_TOKEN});
        } else {
            strconv.push_back({arg[i], CHAR_TOKEN});
        }
    }

    begin = strconv.begin(), end = strconv.end();
}

void ColIt::reset(std::vector<Token> &arg){
    begin = arg.begin(), end = arg.end();
}

void ColIt::offset(const int val) {
    begin = begin + val >= end ? end : begin + val;
}

// CONST ITERATORS // 

RowItConst::RowItConst(const std::vector<std::vector<Token>>& arg){
    begin = arg.begin(), end = arg.end();
}

void RowItConst::offset(const int val) {
    begin = begin + val >= end ? end : begin + val;
}

ColItConst::ColItConst(const std::vector<Token>& arg){
    begin = arg.begin(), end = arg.end();
}

ColItConst::ColItConst(const std::string& arg){
    for(size_t i = 0; i < arg.size(); i++){
        if(arg[i] == ' '){
            strconv.push_back({arg[i], SPACE_TOKEN});
        } else {
            strconv.push_back({arg[i], CHAR_TOKEN});
        }
    }

    begin = strconv.begin(), end = strconv.end();
}

void ColItConst::reset(const std::vector<Token> &arg){
    begin = arg.begin(), end = arg.end();
}

void ColItConst::offset(const int val) {
    begin = begin + val >= end ? end : begin + val;
}