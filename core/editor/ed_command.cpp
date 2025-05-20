#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_sys.hpp"
#include "../../include/core/core_defines.hpp"


void Editor::ed_cmd_ins(const unsigned char c)
{
    if (cmd.cursor <= static_cast<int>(cmd.cmdstr.size())) {
        cmd.cmdstr.insert(cmd.cmdstr.begin() + cmd.cursor, c);
        cmd.cursor++;
    }
}

Editor &Editor::ed_eval_cmd(void)
{
    cmd.sanitized.clear();
    for(size_t i = 0; i < cmd.cmdstr.size(); i++){
        if(cmd.cmdstr[i] != ' '){
            cmd.sanitized += cmd.cmdstr[i];
        }
    }
    ed_clear_cmd();
    return *this;
}

std::pair<std::string, int32_t> Editor::ed_do_cmd(void)
{
}

void Editor::ed_clear_cmd(void){
    cmd.cmdstr.clear();
    cmd.cursor = 0;
}