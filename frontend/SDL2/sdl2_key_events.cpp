#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"

#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_event_mgr.hpp"

EventResult KeyEvent::ev_backspace(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (e->ed_get_mode() == INSERT && SDL_IsTextInputActive()) {
        if(buf){
            buf->buf_rmv_before();
            return EventResult(BUFFER_MUTATION, NO_OPTION, id);
        }
    } else if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        if(buf){
            buf->buf_mv_col(-1);
            return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
        }
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}


EventResult KeyEvent::ev_return(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (e->ed_get_mode() == INSERT && SDL_IsTextInputActive()) {
        if(buf){        
            buf->buf_new_line();
            return EventResult(BUFFER_MUTATION, NO_OPTION, id);
        }
    } else if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        if(buf){
            buf->buf_mv_row(1);
            return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
        }
    } else if (e->ed_get_mode() == CMD && SDL_IsTextInputActive()){
        return EventResult(COMMAND_COMMIT, e->ed_eval_cmd(), id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::ev_delete(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (e->ed_get_mode() == INSERT || e->ed_get_mode() == NAV) {
        if(buf){
            buf->buf_rmv_at();
            return EventResult(BUFFER_MUTATION, NO_OPTION, id);
        }
    } else if(e->ed_get_mode() == CMD){
        if(buf){
            e->ed_clear_cmd();
            return EventResult(COMMAND_MUTATION, NO_OPTION, id);
        }
    }

    return EventResult(NO_KEY, NO_OPTION, id);
}
