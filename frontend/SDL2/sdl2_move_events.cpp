#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"

EventResult KeyEvent::next_char(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if(keymod & controls.ACTION_MOD && !e->ed_taking_cmd() && buf){
        buf->buf_mv_col(1);
        return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::prev_line(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (keymod & controls.ACTION_MOD && !e->ed_taking_cmd() && buf) {
        buf->buf_update_col(buf->buf_mv_row(-1));
        return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::next_line(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (keymod & controls.ACTION_MOD && !e->ed_taking_cmd() && buf) {
        buf->buf_update_col(buf->buf_mv_row(1));
        return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::prev_char(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if(keymod & controls.ACTION_MOD && !e->ed_taking_cmd() && buf){
        buf->buf_mv_col(-1);
        return EventResult(BUFFER_CURSOR_MOVE, NO_OPTION, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}