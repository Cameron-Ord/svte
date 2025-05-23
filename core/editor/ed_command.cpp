#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_defines.hpp"
#include "../../include/core/core_defines.hpp"

#include <array>

const size_t new_identifier = 'n' + 'e' + 'w';
const size_t swb_identifier = 's' + 'w' + 'b';
const size_t fss_identifier = 'f' + 's' + 's';

const std::unordered_set<std::string> cmd_prefixes = {
    "new", "swb", "fss"
};

const std::array<std::pair<std::string, int>, 3> cmd_id_pairs = {
    std::make_pair("new", NEW_BUFFER), std::make_pair("swb", 1), std::make_pair("fss", 2)
};

// 0 prefix 1 arg
Editor& Editor::ed_ins_cmd(const char *c, const size_t size)
{
    switch(cmd.cmd_input_mode){
        case 0:{
            if (cmd.pfx_cursor <= static_cast<int>(cmd.cmd_prefix.size())) {
                for(size_t i = 0; i < size && c[i] != NULL_CHAR; i++){
                    cmd.cmd_prefix.insert(cmd.cmd_prefix.begin() + cmd.pfx_cursor, c[i]);
                    cmd.pfx_cursor++;
                }
            }
        }break;

        case 1:{
            if (cmd.arg_cursor <= static_cast<int>(cmd.cmd_arg.size())) {
                for(size_t i = 0; i < size && c[i] != NULL_CHAR; i++){
                    cmd.cmd_arg.insert(cmd.cmd_arg.begin() + cmd.arg_cursor, c[i]);
                    cmd.arg_cursor++;
                }
            }
        }break;
    }

    return *this;
}

int Editor::ed_cmd_pfx_to_int(void){
    for(size_t i = 0; i < cmd_id_pairs.size(); i++){
        if(cmd.cmd_prefix == cmd_id_pairs[i].first){
            return cmd_id_pairs[i].second;
        }
    }
    return -1;
}


void Editor::ed_eval_cmd(void)
{
    if(cmd_prefixes.count(cmd.cmd_prefix)){
        ed_set_arg_cmd_mode();
    } else {
        ed_set_prefix_cmd_mode();
    } 
}

int Editor::ed_cmd_commit(void){
    if(cmd.cmd_input_mode != 1){
        return cmd.CMD_INVALID;
    }
    return ed_cmd_pfx_to_int();
}



