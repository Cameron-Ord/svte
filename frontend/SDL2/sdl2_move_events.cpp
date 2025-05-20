#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"

EventResult KeyEvent::ev_left(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (!buf) {
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    if(keymod & KMOD_LCTRL){
        if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
            e->ed_set_current_id(e->ed_prev_id());
            return EventResult(keys.ch_buffer, opts.no_opt, id);
        }
    } else {
        if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
            buf->buf_mv_col(-1);
            return EventResult(keys.buf_cursor_move, opts.no_opt, id);
        }
    }
    return EventResult(keys.unbound, opts.no_opt, id);
}

EventResult KeyEvent::ev_up(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (!buf) {
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        buf->buf_update_col(buf->buf_mv_row(-1));
    }
    return EventResult(keys.buf_cursor_move, opts.no_opt, id);
}

EventResult KeyEvent::ev_down(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (!buf) {
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        buf->buf_update_col(buf->buf_mv_row(1));
    }
    return EventResult(keys.buf_cursor_move, opts.no_opt, id);
}

EventResult KeyEvent::ev_right(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (!buf) {
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    if(keymod & KMOD_LCTRL){
        if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
            e->ed_set_current_id(e->ed_next_id());
            return EventResult(keys.ch_buffer, opts.no_opt, id);
        }
    } else {
        if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
            buf->buf_mv_col(1);
            return EventResult(keys.buf_cursor_move, opts.no_opt, id);
        }
    }

    return EventResult(keys.unbound, opts.no_opt, id);
}