#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_error_codes.hpp"

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
    if(!cmd.sanitized.empty()){
        switch(cmd.sanitized[0]){
            default:{
                return {"noopt", -1};
            }
            case 'n':{
                cmd.sanitized.erase(cmd.sanitized.begin());
                return {"newfile", ed_commit_buffer(cmd.sanitized)};
            }break;
        }
    } else {
        return {"noopt", -1};
    }
}

void Editor::ed_clear_cmd(void){
    cmd.cmdstr.clear();
    cmd.cursor = 0;
}