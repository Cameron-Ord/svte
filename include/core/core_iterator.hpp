#ifndef CORE_ITERATOR_HPP
#define CORE_ITERATOR_HPP

#include <vector>
#include <string>

// Just defining structs for encapsulating iterators.
// Defined in core include, can be safely included anywhere.

struct ConstBufRowIt {
    const std::vector<std::string>& row;
    std::vector<std::string>::const_iterator end;
    std::vector<std::string>::const_iterator begin;
    std::vector<std::string>::const_reverse_iterator rend;
    std::vector<std::string>::const_reverse_iterator rbegin;
    
    ConstBufRowIt(const std::vector<std::string> &row) : row(row){
        init_iterators();
    }

    void init_iterators(void){
        end = row.end(), begin = row.begin();
        rend = row.rend(), rbegin = row.rbegin();
    }

    ConstBufRowIt& offset(const int val){ 
        begin = row.begin() + val; 
        return *this;
    }
    
    ConstBufRowIt& roffset(const int val){ 
        rbegin = row.rbegin() + val;
        return *this;
    }
    
    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void rvalid(void){
        if(rbegin >= rend){
            rbegin = rend;
        }
    }

    void increment(void){ ++begin; }
    void rincrement(void){ ++rbegin; }
    void decrement(void){ --begin; }
    void rdecrement(void){ --rbegin; }
};

struct ConstBufStrIt{
    const std::string& line;

    std::string::const_iterator end;
    std::string::const_iterator begin;
    std::string::const_reverse_iterator rend;
    std::string::const_reverse_iterator rbegin;
    
    ConstBufStrIt(const std::string &line) : line(line){
        init_iterators();
    }

    void init_iterators(void){
        end = line.end(), begin = line.begin();
        rend = line.rend(), rbegin = line.rbegin();
    }

    ConstBufStrIt& offset(const int val){ 
        begin = line.begin() + val; 
        return *this;
    }

    ConstBufStrIt& roffset(const int val){ 
        rbegin = line.rbegin() + val;
        return *this;
    }

    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void rvalid(void){
        if(rbegin >= rend){
            rbegin = rend;
        }
    }

    void increment(void){ ++begin; }
    void rincrement(void){ ++rbegin; }
    void decrement(void){ --begin; }
    void rdecrement(void){ --rbegin; }
};

struct BufRowIt {
    std::vector<std::string>& row;
    std::vector<std::string>::iterator end;
    std::vector<std::string>::iterator begin;
    std::vector<std::string>::reverse_iterator rend;
    std::vector<std::string>::reverse_iterator rbegin;
    
    BufRowIt(std::vector<std::string> &row) : row(row){
        init_iterators();
    }

    void init_iterators(void){
        end = row.end(), begin = row.begin();
        rend = row.rend(), rbegin = row.rbegin();
    }

    BufRowIt& offset(const int val){ 
        begin = row.begin() + val; 
        return *this;
    }

    BufRowIt& roffset(const int val){ 
        rbegin = row.rbegin() + val; 
        return *this;
    }

    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void rvalid(void){
        if(rbegin >= rend){
            rbegin = rend;
        }
    }
    
    void increment(void){ ++begin; }
    void rincrement(void){ ++rbegin; }
    void decrement(void){ --begin; }
    void rdecrement(void){ --rbegin; }
};

struct BufStrIt{
    std::string& line;

    std::string::iterator end;
    std::string::iterator begin;
    std::string::reverse_iterator rend;
    std::string::reverse_iterator rbegin;
    
    BufStrIt(std::string &line) : line(line){
        init_iterators();
    }

    void init_iterators(void){
        end = line.end(), begin = line.begin();
        rend = line.rend(), rbegin = line.rbegin();
    }

    BufStrIt& offset(const int val){ 
        begin = line.begin() + val; 
        return *this;
    }

    BufStrIt& roffset(const int val){ 
        rbegin = line.rbegin() + val; 
        return *this;
    }

    void valid(void){
        if(begin >= end){
            begin = end;
        }
    }

    void rvalid(void){
        if(rbegin >= rend){
            rbegin = rend;
        }
    }

    void increment(void){ ++begin; }
    void rincrement(void){ ++rbegin; }
    void decrement(void){ --begin; }
    void rdecrement(void){ --rbegin; }
};
#endif