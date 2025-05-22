#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_defines.hpp"


#include <iostream>

Editor::Editor(void) : error(CORE_NIL), current_buffer_id(CORE_NIL), cmd_mode(0)
{
    cmd.cursor = 0;
}

Editor::~Editor(void)
{
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
