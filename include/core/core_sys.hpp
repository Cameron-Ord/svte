#ifndef CORE_SYS_HPP
#define CORE_SYS_HPP

// Responsible for managing paths - reading/writing/opening/saving files.

#include <cstdint>
#include <unordered_set>
#include <filesystem>
#include <vector>
#include <string>

typedef enum { FS_ERR = 0, FS_NIL = -1} FS_ERROR_CODE; 

class FileSys{

    public:
        FileSys(void);
        
        std::pair<std::string, std::vector<std::string>> sys_new_file(const std::string fn);
        std::vector<std::string> sys_read(const std::string path);
        const size_t sys_write(const std::string path, std::vector<std::string> buffer);
        void sys_set_err(const int errval) { error = errval; }
        const int sys_get_err(void) { return error; }
        int sys_current_path(void);
        int sys_check_exists(const std::string pathstr);
        void sys_set_cwd(std::filesystem::path d) { cwd = d; }
        std::string sys_delimiter(void);
        std::string sys_cwd_join_fn(const std::string fn);



    private:
        int error;
        //Used to avoid duplicates and validate on save
        std::unordered_set<std::string> open_paths;
        std::filesystem::path cwd;


};

#endif