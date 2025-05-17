#ifndef CORE_ITERATOR_HPP
#define CORE_ITERATOR_HPP

#include <vector>
#include <string>

// Just defining structs for encapsulating iterators.
// Defined in core include, can be safely included anywhere.

struct RConstRangeStr{
    std::string::const_reverse_iterator rend;
    std::string::const_reverse_iterator rbegin;

    RConstRangeStr(
        std::string::const_reverse_iterator e,
        std::string::const_reverse_iterator b 
    ) : rend(e), rbegin(b >= e ? e : b) {} 
    
    std::string::const_reverse_iterator _begin(){ return rbegin; }
    std::string::const_reverse_iterator _end() { return rend; }
};

struct ConstRangeStr{
    std::string::const_iterator end;
    std::string::const_iterator begin;

    ConstRangeStr(
        std::string::const_iterator e,
        std::string::const_iterator b 
    ) : end(e), begin(b >= e ? e : b) {} 
    
    std::string::const_iterator _begin(){ return begin; }
    std::string::const_iterator _end() { return end; }
};

struct ConstRangeVecStr{
    std::vector<std::string>::const_iterator end;
    std::vector<std::string>::const_iterator begin;

    ConstRangeVecStr(
        std::vector<std::string>::const_iterator e,
        std::vector<std::string>::const_iterator b 
    ) : end(e), begin(b >= e ? e : b) {} 
    
    std::vector<std::string>::const_iterator _begin(){ return begin; }
    std::vector<std::string>::const_iterator _end() { return end; }
};

struct MutRangeStr{
    std::string::iterator end;
    std::string::iterator begin;

    MutRangeStr(
        std::string::iterator e,
        std::string::iterator b 
    ) : end(e), begin(b >= e ? e : b) {} 
    
    std::string::iterator _begin(){ return begin; }
    std::string::iterator _end() { return end; }
};

struct MutRangeVecStr{
    std::vector<std::string>::iterator end;
    std::vector<std::string>::iterator begin;

    MutRangeVecStr(
        std::vector<std::string>::iterator e,
        std::vector<std::string>::iterator b 
    ) : end(e), begin(b >= e ? e : b){} 
    
    std::vector<std::string>::iterator _begin(){ return begin; }
    std::vector<std::string>::iterator _end() { return end; }
};


#endif