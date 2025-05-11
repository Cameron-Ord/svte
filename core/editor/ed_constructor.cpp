#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_error_codes.hpp"


#include <iostream>

Editor::Editor(void) : error(CORE_NIL), mode(NVISUAL), current_buffer_id(CORE_NIL)  {
    ed_set_error(ed_determine_cwd());
    std::cout << "Editor Path: " << ed_working_path << std::endl;
}

Editor::~Editor(void){
    
}

const int32_t&  Editor::ed_get_current_id(void){
    return current_buffer_id;
}

void Editor::ed_set_current_id(const int32_t id){
    current_buffer_id = id;
}

void Editor::ed_set_error(const int err){
    error = err;
}

const int& Editor::ed_get_error(void){
    return error;
}