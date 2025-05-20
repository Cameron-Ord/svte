#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/core/core_editor.hpp"

EventResult KeyEvent::ev_visual(const int &keymod, Editor *e, const int32_t id)
{
    Buffer *buf = e->ed_fetch_buffer(id);
    if (!buf) {
        return EventResult(keys.unbound, opts.no_opt, id);
    }

    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        e->ed_set_mode(SELECT);
    }
    return EventResult(keys.unbound, opts.no_opt, id);
}

EventResult KeyEvent::ev_append(const int &keymod, Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        e->ed_set_mode(APPEND);
        return EventResult(keys.input_mode, opts.start_text_input, id);
    }
    return EventResult(keys.unbound, opts.no_opt, id);
}

EventResult KeyEvent::ev_insert(const int &keymod, Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        e->ed_set_mode(INSERT);
        return EventResult(keys.input_mode, opts.start_text_input, id);
    }
    return EventResult(keys.unbound, opts.no_opt, id);
}

EventResult KeyEvent::ev_cmd(const int &keymod, Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        e->ed_set_mode(CMD);
        return EventResult(keys.input_mode, opts.start_text_input, id);
    }
    return EventResult(keys.unbound, opts.no_opt, id);
}

EventResult KeyEvent::ev_escape(const int &keymod, Editor *e, const int32_t id)
{
    // No check, if this key is hit always just back out of whatever mode it was in no matter what.
    e->ed_set_mode(NAV);
    if (SDL_IsTextInputActive()) {
        return EventResult(keys.input_mode, opts.stop_text_input, id);
    }
    return EventResult(keys.unbound, opts.no_opt, id);
}
