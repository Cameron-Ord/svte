#ifndef STATUS_HPP
#define STATUS_HPP

#include <string>
#include <unordered_map>

struct Status_Data{
    std::string filename;
    std::string cmd;
    std::string path;
    std::string time;
    int buffer_index;
};
typedef struct Status_Data Status_Data;

class Status {
    public:
        const Status_Data* status_get_sd_ptr(const int32_t);
        
    private:
        std::unordered_map<int32_t, Status_Data> status;
                   
};

#endif
