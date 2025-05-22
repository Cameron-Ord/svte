#ifndef CORE_ITERATOR_HPP
#define CORE_ITERATOR_HPP

#include <vector>
#include <string>

struct Token {
    std::string token;
    std::string identifier;
};

struct ConstTokenRowIt {
    const std::vector<Token>& tokens;
    int substr_offset = 0;


    std::vector<Token>::const_iterator end;
    std::vector<Token>::const_iterator begin;

    ConstTokenRowIt(const std::vector<Token> &row) : tokens(row){
        init_iterators();
    }

    void init_iterators(void){
        end = tokens.end(), begin = tokens.begin();
    }

    const int map_offset(const int val){
        int index = 0;
        const int size = static_cast<int>(tokens.size());
        
        for(int i = 0; i < size; i++){
            const int len = static_cast<int>(tokens[i].token.size());
            if(index + len > val){
                substr_offset = val - index;
                return static_cast<int>(i);
            }
            index += len;
        }

        substr_offset = 0;
        return static_cast<int>(tokens.size());
    }

    ConstTokenRowIt& offset(const int val){    
        begin = tokens.begin() + map_offset(val); 
        return *this;
    }
    
    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void increment(void){ ++begin; }
    void decrement(void){ --begin; }
};

struct ConstBufRowIt {
    const std::vector<std::string>& row;
    std::vector<std::string>::const_iterator end;
    std::vector<std::string>::const_iterator begin;
    
    ConstBufRowIt(const std::vector<std::string> &row) : row(row){
        init_iterators();
    }

    void init_iterators(void){
        end = row.end(), begin = row.begin();
    }

    ConstBufRowIt& offset(const int val){ 
        begin = row.begin() + val; 
        return *this;
    }
        
    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void increment(void){ ++begin; }
    void decrement(void){ --begin; }
};

struct ConstBufStrIt{
    const std::string& line;

    std::string::const_iterator end;
    std::string::const_iterator begin;
    
    ConstBufStrIt(const std::string &line) : line(line){
        init_iterators();
    }

    void init_iterators(void){
        end = line.end(), begin = line.begin();
    }

    ConstBufStrIt& offset(const int val){ 
        begin = line.begin() + val; 
        return *this;
    }

    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void increment(void){ ++begin; }
    void decrement(void){ --begin; }
};

struct BufRowIt {
    std::vector<std::string>& row;
    std::vector<std::string>::iterator end;
    std::vector<std::string>::iterator begin;
    
    BufRowIt(std::vector<std::string> &row) : row(row){
        init_iterators();
    }

    void init_iterators(void){
        end = row.end(), begin = row.begin();
    }

    BufRowIt& offset(const int val){ 
        begin = row.begin() + val; 
        return *this;
    }

    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void increment(void){ ++begin; }
    void decrement(void){ --begin; }
};

struct BufStrIt{
    std::string& line;

    std::string::iterator end;
    std::string::iterator begin;
    
    BufStrIt(std::string &line) : line(line){
        init_iterators();
    }

    void init_iterators(void){
        end = line.end(), begin = line.begin();
    }

    BufStrIt& offset(const int val){ 
        begin = line.begin() + val; 
        return *this;
    }

    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void increment(void){ ++begin; }
    void decrement(void){ --begin; }
};
#endif