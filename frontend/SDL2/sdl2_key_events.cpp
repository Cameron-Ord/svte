#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"

EventResult KeyEvent::ev_backspace(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (!buf) {
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    if (e->ed_get_mode() == INSERT && SDL_IsTextInputActive()) {
        buf->buf_rmv_before();
        return EventResult(keys.text_input, opts.no_opt, id);

    } else if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        buf->buf_mv_col(-1);
        return EventResult(keys.buf_cursor_move, opts.no_opt, id);
    }

    return EventResult(keys.unbound, opts.no_opt, id);
}


EventResult KeyEvent::ev_return(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (!buf) {
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    if (e->ed_get_mode() == INSERT && SDL_IsTextInputActive()) {
        buf->buf_new_line();
        return EventResult(keys.text_input, opts.no_opt, id);

    } else if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        buf->buf_mv_row(1);
        return EventResult(keys.buf_cursor_move, opts.no_opt, id);
    }

    return EventResult(keys.unbound, opts.no_opt, id);
}

EventResult KeyEvent::ev_delete(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (!buf) {
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    if (e->ed_get_mode() == INSERT || e->ed_get_mode() == NAV) {
        buf->buf_rmv_at();
        return EventResult(keys.text_input, opts.no_opt, id);
    } else if(e->ed_get_mode() == CMD){
        e->ed_clear_cmd();
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    return EventResult(keys.unbound, opts.no_opt, id);
}
