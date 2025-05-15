#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_error_codes.hpp"

KeyEvent::KeyEvent(void) : controls(ev_init_controls())
{
    ev_init_keybinds();
    std::cout << "KeyEvents initialized" << std::endl;
}

Controls KeyEvent::ev_init_controls(void)
{
    Controls c = {
        controls.LEFT = SDLK_h,
        controls.RIGHT = SDLK_l,
        controls.UP = SDLK_k,
        controls.DOWN = SDLK_j,

        controls.INSERT_MODE = SDLK_i,
        controls.APPEND_MODE = SDLK_a,
        controls.VISUAL_MODE = SDLK_v,
        controls.ESCAPE = SDLK_ESCAPE,

        controls.ACTION_CMD = SDLK_c,
        controls.ACTION_MOD = KMOD_LCTRL,
    };
    return c;
}

//https://en.cppreference.com/w/cpp/language/lambda
void KeyEvent::ev_init_keybinds(void)
{
    binds.insert({controls.LEFT, [this](int mod, class Editor *e, const int id) -> EventRet { return ev_left(mod, e, id); }});
    binds.insert({controls.RIGHT, [this](int mod, class Editor *e, const int id) -> EventRet { return ev_right(mod, e, id); }});
    binds.insert({controls.UP, [this](int mod, class Editor *e, const int id) -> EventRet { return ev_up(mod, e, id); }});
    binds.insert({controls.DOWN, [this](int mod, class Editor *e, const int id) -> EventRet { return ev_down(mod, e, id); }});
    binds.insert({controls.INSERT_MODE, [this](int mod, class Editor *e, const int id) -> EventRet { return ev_insert(mod, e, id); }});
    binds.insert({controls.APPEND_MODE, [this](int mod, class Editor *e, const int id) -> EventRet { return ev_append(mod, e, id); }});
    binds.insert({controls.ESCAPE, [this](int mod, class Editor *e, const int id) -> EventRet { return ev_escape(mod, e, id); }});
    binds.insert({controls.VISUAL_MODE, [this](int mod, class Editor *e, const int id) -> EventRet { return ev_visual(mod, e, id); }});
}

EventRet KeyEvent::ev_left(const int &keymod, class Editor *e, const int id)
{
    if (e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT) {
        e->ed_mv_cursor_col(id, -1);
    }
    EventRet ret = {id, SDL2_NIL, e->ed_get_col(id), e->ed_get_row(id)};
    return ret;
}

EventRet KeyEvent::ev_up(const int &keymod, class Editor *e, const int id)
{
    if (e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT) {
        e->ed_mv_cursor_row(id, -1);
    }
    EventRet ret = {id, SDL2_NIL, e->ed_get_col(id), e->ed_get_row(id)};
    return ret;
}

EventRet KeyEvent::ev_down(const int &keymod, class Editor *e, const int id)
{
    if (e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT) {
        e->ed_mv_cursor_row(id, 1);
    }
    EventRet ret = {id, SDL2_NIL, e->ed_get_col(id), e->ed_get_row(id)};
    return ret;
}

EventRet KeyEvent::ev_right(const int &keymod, class Editor *e, const int id)
{
    if (e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT) {
        e->ed_mv_cursor_col(id, 1);
    }
    EventRet ret = {id, SDL2_NIL, e->ed_get_col(id), e->ed_get_row(id)};
    return ret;
}

EventRet KeyEvent::ev_visual(const int &keymod, class Editor *e, const int id)
{
    if (e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT) {
        e->ed_set_mode(VISUAL);
    }
    EventRet ret = {id, SDL2_NIL, SDL2_NIL, SDL2_NIL};
    return ret;
}

EventRet KeyEvent::ev_append(const int &keymod, class Editor *e, const int id)
{
    if (e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT) {
        e->ed_set_mode(APPEND);
        e->ed_set_edit(EDIT);
        EventRet ret = {id, START_INPUT, SDL2_NIL, SDL2_NIL};
        return ret;
    }
    EventRet ret = {id, SDL2_NIL, SDL2_NIL, SDL2_NIL};
    return ret;
}

EventRet KeyEvent::ev_insert(const int &keymod, class Editor *e, const int id)
{
    if (e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT) {
        e->ed_set_mode(INSERT);
        e->ed_set_edit(EDIT);
        EventRet ret = {id, START_INPUT, SDL2_NIL, SDL2_NIL};
        return ret;
    }
    EventRet ret = {id, SDL2_NIL, SDL2_NIL, SDL2_NIL};
    return ret;
}

EventRet KeyEvent::ev_escape(const int &keymod, class Editor *e, const int id)
{
    e->ed_set_mode(NVISUAL);
    e->ed_set_edit(NO_EDIT);
    EventRet ret = {id, STOP_INPUT, SDL2_NIL, SDL2_NIL};
    return ret;
}

EventRet KeyEvent::ev_mainloop_keydown(const int keysym, const int keymod, class Editor *e, const int id)
{
    std::unordered_map<int, std::function<EventRet(int, class Editor *e, int)>>::const_iterator it = binds.find(keysym);
    if (it != binds.end()) {
        return it->second(keymod, e, id);
    }
    EventRet ret = {id, SDL2_NIL, SDL2_NIL, SDL2_NIL};
    return ret;
}

int KeyEvent::ev_mainloop_poll_event_type(SDL_Event *const e)
{
    while (SDL_PollEvent(e)) {
        return e->type;
    }
    return SDL2_NIL;
}
//This will probably have to have its own return type later possibly, but for now - checking these two events for size changes is fine.
int KeyEvent::ev_mainloop_window_event_type(const int windowevent)
{
    switch (windowevent) {
    default:
    {
        return 0;
    }
    case SDL_WINDOWEVENT_RESIZED:
    {
        return 1;
    } break;

    case SDL_WINDOWEVENT_SIZE_CHANGED:
    {
        return 1;
    } break;
    }
}

void KeyEvent::ev_mainloop_text_input(const char *text, class Editor *e, const int id)
{
    const size_t len = strlen(text);
    for (size_t i = 0; i < len; i++) {
        e->ed_ins_char(id, text[i]);
    }
}

int KeyEvent::ev_mainloop_die(void) { return NO_RUN; }
