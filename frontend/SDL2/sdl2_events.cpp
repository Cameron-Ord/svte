#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_events.hpp"

#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_editor.hpp"

/*
Right now the setup I have for the event chain is really holding me back, 
it has the ground work for being good but I need to decouple it from the tight grip of SDL and use a core event to handle sdl2 or core changes from the result

For example if I want to do a command right now its next to impossible as I have to return a key and opt value that I defined in an sdl2 header and that cant be included in core files.
Also would have to pass another class pointer for the sys which just should not be done IMO. So yeah fix this shit you lazy cunt.
*/

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
        controls.RETURN = SDLK_RETURN,
        controls.BACKSPACE = SDLK_BACKSPACE,
        controls.DELETE = SDLK_DELETE,

        controls.INSERT_MODE = SDLK_i,
        controls.APPEND_MODE = SDLK_a,
        controls.SELECTION_MODE = SDLK_v,
        controls.NAV_MODE = SDLK_ESCAPE,
        controls.ACTION_CMD = SDLK_c,

        controls.ACTION_MOD = KMOD_LCTRL,
    };
    return c;
}

//https://en.cppreference.com/w/cpp/language/lambda
void KeyEvent::ev_init_keybinds(void)
{
    binds.insert({controls.LEFT, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_left(mod, e, id); }});
    binds.insert({controls.RIGHT, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_right(mod, e, id); }});
    binds.insert({controls.UP, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_up(mod, e, id); }});
    binds.insert({controls.DOWN, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_down(mod, e, id); }});

    binds.insert({controls.RETURN, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_return(mod, e, id); }});
    binds.insert({controls.BACKSPACE, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_backspace(mod, e, id); }});
    binds.insert({controls.DELETE, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_delete(mod, e, id); }});

    binds.insert({controls.INSERT_MODE, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_insert(mod, e, id); }});
    binds.insert({controls.APPEND_MODE, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_append(mod, e, id); }});
    binds.insert({controls.NAV_MODE, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_escape(mod, e, id); }});
    binds.insert({controls.SELECTION_MODE, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_visual(mod, e, id); }});
    binds.insert({controls.ACTION_CMD, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_cmd(mod, e, id); }});
}


EventResult KeyEvent::ev_mainloop_keydown(const int keysym, const int keymod, Editor *e)
{
    const int32_t id = e->ed_get_current_id();
    std::unordered_map<int, std::function<EventResult(int, Editor *e, int)>>::const_iterator it;
    it = binds.find(keysym);
    if (it != binds.end()) {
        return it->second(keymod, e, id);
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}

int KeyEvent::ev_mainloop_poll_event_type(SDL_Event *const e)
{
    while (SDL_PollEvent(e)) {
        return e->type;
    }
    return SDL2_NIL;
}

EventResult KeyEvent::ev_mainloop_window_event_type(const int windowevent)
{
    switch (windowevent) {
    default:
    {
        return EventResult(NO_KEY, NO_OPTION, SDL2_NIL);
    }
    case SDL_WINDOWEVENT_RESIZED:
    {
        return EventResult(DISPLAY_SIZE_CHANGED, DISPLAY_UPDATE, SDL2_NIL);
    } break;

    case SDL_WINDOWEVENT_SIZE_CHANGED:
    {
        return EventResult(DISPLAY_SIZE_CHANGED, DISPLAY_UPDATE, SDL2_NIL);
    } break;
    }
}

EventResult KeyEvent::ev_mainloop_text_input(const char *text, Editor *e, const int32_t id)
{
    //In any of these functions where im checking this condition and it fails it will
    //pretty much prevent any other behavior from occuring - IE. wanting to enter a command without
    //a dedicated buffer present. Obviously I need to check the location of the pointer checks to allow behaviors but im lazy right now.
    Buffer *buf = e->ed_fetch_buffer(id);

    const size_t len = strlen(text);
    for (size_t i = 0; i < len; i++) {
        switch (e->ed_get_mode()) {
        default:
            break;
        case INSERT:
        {
            if (buf) {
                buf->buf_ins_char(text[i]);
            } 
        } break;

        case CMD:
        {
            e->ed_cmd_ins(text[i]);
        } break;
        }
    }

    switch (e->ed_get_mode()) {
    default:
    {
        return EventResult(NO_KEY, NO_OPTION, id);
    }

    case INSERT:
    {
        return EventResult(BUFFER_MUTATION, NO_OPTION, id);
    }

    case CMD:
    {
        return EventResult(COMMAND_MUTATION, NO_OPTION, id);
    }
    }
}

int KeyEvent::ev_mainloop_die(void) { return NO_RUN; }
