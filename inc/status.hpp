#ifndef STATUS_HPP
#define STATUS_HPP

#include <string>
#include <cstdint>
#include <unordered_map>

struct Status_Data{
    std::string filename;
    std::string cmd;
    std::string path;
    std::string time;
};
typedef struct Status_Data Status_Data;

class Status {
    public:
        const Status_Data* status_get_sd_ptr(const int32_t id);
        int status_set_sd_cmd(const int32_t id, std::string cmd);
        int status_set_sd_time(const int32_t id, std::string time);
        
    private:
        std::unordered_map<int32_t, Status_Data> status;
                   
};

#endif
