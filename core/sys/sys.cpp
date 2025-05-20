#include "../../include/core/core_sys.hpp"
#include "../../include/core/core_defines.hpp"

#include <iostream>
#include <fstream>

FileSys::FileSys(void) : error(FS_NIL) {
    sys_set_err(sys_current_path());
}

int FileSys::sys_current_path(void)
{
    try {
        sys_set_cwd(std::filesystem::current_path());
        return FS_NIL;
    } catch (const std::filesystem::filesystem_error &err) {
        std::cerr << "Could not fetch filesystem path -> " << err.what() << std::endl;
        return FS_ERR;
    } catch (...) {
        std::cerr << "Could not fetch filesystem path (unknown error)." << std::endl;
        return FS_ERR;
    }
}

int FileSys::sys_check_exists(const std::string fn){
    if(std::filesystem::exists(fn)){
        std::cout << "Path: " << fn << " Does exist" << std::endl;
        return 1;
    } else {
        std::cout << "Path: " << fn << " Does not exist" << std::endl;
        return 0;
    }
}

std::string FileSys::sys_cwd_join_fn(const std::string fn){
    return cwd.string() +  sys_delimiter() + fn;
}

std::pair<std::string, std::vector<std::string>> FileSys::sys_new_file(const std::string fn){
    const std::string path = sys_cwd_join_fn(fn);

    switch(sys_check_exists(fn)){
        default: {
            return std::make_pair(fn, std::vector<std::string>({""}));
        }break;

        case 1:{
            return std::make_pair(fn, sys_read(path));
        }break;
    }
}

std::vector<std::string> FileSys::sys_read(const std::string path){
    std::ifstream file(path);

    if(!file.is_open()){
        std::cerr << "Failed to open file!" << std::endl;
        return std::vector<std::string>({""});
    }

    std::vector<std::string> buffer;
    std::string line;
    int line_count = 0;
    int byte_accumulator = 0;

    while (std::getline(file, line)) {
        buffer.push_back(line);
        byte_accumulator += line.size() * sizeof(line[0]);
        line_count++;
    }
    file.close();

    const int cond = byte_accumulator > 0;
    switch(cond){
        default:{
            return std::vector<std::string>({""});
        }break;

        case 1:{
            return buffer;
        }break;
    }
}

//return n bytes
const size_t FileSys::sys_write(const std::string path, std::vector<std::string> buffer){
    size_t accumulator = 0;
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return accumulator;
    }

    for (size_t i = 0; i < buffer.size(); i++) {
        accumulator += buffer[i].size() * sizeof(buffer[i][0]);
        //Just needs a newline, so not using std::endl;
        file << buffer[i] << '\n';
    }

    file.close();
    return accumulator;
}



std::string FileSys::sys_delimiter(void)
{
#ifdef _WIN32
    return std::string("\\");
#elif __linux__
    return std::string("/");
#else
    return std::string("");
#endif
}
