#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_event_mgr.hpp"


void Editor::ed_cmd_ins(const char *c, const size_t size)
{
    if (cmd.cursor <= static_cast<int>(cmd.cmdstr.size())) {
        for(size_t i = 0; i < size && c[i] != NULL_CHAR; i++){
            cmd.cmdstr.insert(cmd.cmdstr.begin() + cmd.cursor, c[i]);
            cmd.cursor++;
        }
    }
}

int Editor::ed_eval_cmd(void)
{
    //Extremely basic but works for now
    cmd.sanitized.clear();
    for(size_t i = 0; i < cmd.cmdstr.size(); i++){
        if(cmd.cmdstr[i] != ' '){
            cmd.sanitized += cmd.cmdstr[i];
        }
    }
    cmd.cmdstr.clear();
    cmd.cursor = 0;

    if(cmd.sanitized.empty()){
        return NO_OPTION;
    }

    switch(cmd.sanitized[0]){
        default:{
            return NO_OPTION;
        }

        case 'n': {
            cmd.sanitized.erase(cmd.sanitized.begin());
            return NEW_BUFFER;
        }
    }
}