#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"

EventResult KeyEvent::ev_left(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if(keymod & KMOD_LCTRL){
        if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
            e->ed_set_current_id(e->ed_prev_id());
            return EventResult(NO_KEY, NO_OPTION, id);
        }
    } else {
        if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive() && buf != nullptr) {
            buf->buf_mv_col(-1);
            return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
        }
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::ev_up(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        if(buf){
            buf->buf_update_col(buf->buf_mv_row(-1));
            return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
        }
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::ev_down(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        if(buf){
            buf->buf_update_col(buf->buf_mv_row(1));
            return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
        }
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::ev_right(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if(keymod & KMOD_LCTRL){
        if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
            e->ed_set_current_id(e->ed_next_id());
            return EventResult(NO_KEY, NO_OPTION, id);
        }
    } else {
        if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive() && buf != nullptr) {
            buf->buf_mv_col(1);
            return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
        }
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}