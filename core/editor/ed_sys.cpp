#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_error_codes.hpp"

#include <iostream>
#include <filesystem>

int Editor::ed_determine_cwd(void){
    std::string cwd;
    if((cwd = ed_fs_cwd_string()).empty()){
        return BAD_PATH;
    } else {
        ed_set_cwd(cwd);
        return CORE_NIL;
    }
}

const std::string& Editor::ed_get_cwd(void){
    return ed_working_path;
}

void Editor::ed_set_cwd(std::string str){
    ed_working_path = str;
}

std::string Editor::ed_fs_cwd_string(void){
    try {
        return std::filesystem::current_path().string();
    } catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "Could not fetch filesystem path -> " << err.what() << std::endl;
        return std::string("");
    } catch (...){
        std::cerr << "Could not fetch filesystem path (unknown error)." << std::endl;
        return std::string("");
    }
}

std::string Editor::ed_delimiter(void){
#ifdef _WIN32
    return std::string("\\");
#elif __linux__
    return std::string("/");
#else 
    return std::string("");
#endif
}
