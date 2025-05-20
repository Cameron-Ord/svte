#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_defines.hpp"


#include <iostream>

Editor::Editor(void) : error(CORE_NIL), mode(NAV), current_buffer_id(CORE_NIL)
{
    cmd.cursor = 0;
}

Editor::~Editor(void)
{
}

const std::string Editor::ed_mode_str(const int8_t c) const {
    switch(c){
        default: {
            return std::string("");
        }break;

        case NAV: {
            return std::string("nav");
        }break;

        case SELECT:{
            return std::string("select");
        }break;

        case INSERT:{
            return std::string("insert");
        }break;

        case CMD:{
            return std::string("cmd");
        }break;
    }
}

void Editor::ed_set_mode(const uint8_t val)
{
    mode = val;
}

const int32_t &Editor::ed_get_current_id(void)
{
    return current_buffer_id;
}

void Editor::ed_set_current_id(const int32_t id)
{
    current_buffer_id = id;
}

void Editor::ed_set_error(const int err)
{
    error = err;
}

const int &Editor::ed_get_error(void)
{
    return error;
}
