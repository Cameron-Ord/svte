#ifndef CORE_ITERATOR_HPP
#define CORE_ITERATOR_HPP

#include <vector>
#include <string>

struct ConstRangeStr{
    std::string::const_iterator begin;
    std::string::const_iterator end;

    ConstRangeStr(
        std::string::const_iterator b, 
        std::string::const_iterator e
    ) : begin(b), end(e){} 
    
    std::string::const_iterator begin(){ return begin; }
    std::string::const_iterator end() { return end; }
};

struct ConstRangeVecStr{
    std::vector<std::string>::const_iterator begin;
    std::vector<std::string>::const_iterator end;

    ConstRangeVecStr(
        std::vector<std::string>::const_iterator b, 
        std::vector<std::string>::const_iterator e
    ) : begin(b), end(e){} 
    
    std::vector<std::string>::const_iterator begin(){ return begin; }
    std::vector<std::string>::const_iterator end() { return end; }
};

struct MutRangeStr{
    std::string::iterator begin;
    std::string::iterator end;

    MutRangeStr(
        std::string::iterator b, 
        std::string::iterator e
    ) : begin(b), end(e){} 
    
    std::string::iterator begin(){ return begin; }
    std::string::iterator end() { return end; }
};

struct MutRangeVecStr{
    std::vector<std::string>::iterator begin;
    std::vector<std::string>::iterator end;

    MutRangeVecStr(
        std::vector<std::string>::iterator b, 
        std::vector<std::string>::iterator e
    ) : begin(b), end(e){} 
    
    std::vector<std::string>::iterator begin(){ return begin; }
    std::vector<std::string>::iterator end() { return end; }
};


#endif