#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/core/core_editor.hpp"

EventResult KeyEvent::ev_visual(const int &keymod, Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        e->ed_set_mode(SELECT);
        return EventResult(MODE_CHANGE, NO_OPTION, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::ev_append(const int &keymod, Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        e->ed_set_mode(APPEND);
        return EventResult(MODE_CHANGE, START_TEXT_INPUT, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::ev_insert(const int &keymod, Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        e->ed_set_mode(INSERT);
        return EventResult(MODE_CHANGE, START_TEXT_INPUT, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::ev_cmd(const int &keymod, Editor *e, const int32_t id)
{
    if (e->ed_get_mode() == NAV && !SDL_IsTextInputActive()) {
        e->ed_set_mode(CMD);
        return EventResult(MODE_CHANGE, START_TEXT_INPUT, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

EventResult KeyEvent::ev_escape(const int &keymod, Editor *e, const int32_t id)
{
    // No check, if this key is hit always just back out of whatever mode it was in no matter what.
    e->ed_set_mode(NAV);
    if (SDL_IsTextInputActive()) {
        return EventResult(MODE_CHANGE, STOP_TEXT_INPUT, id);
    }
    return EventResult(MODE_CHANGE, NO_OPTION, id);
}
