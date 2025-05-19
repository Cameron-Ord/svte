#include "../../include/core/core_editor.hpp"


void Editor::ed_cmd_ins(const unsigned char c){
    if(cmd.cursor <= static_cast<int>(cmd.cmdstr.size())){
        cmd.cmdstr.insert(cmd.cmdstr.begin() + cmd.cursor, c);
        cmd.cursor++;
    }
}

Editor& Editor::ed_eval_cmd(void){

}

void Editor::ed_do_cmd(void){
    
}