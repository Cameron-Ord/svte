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
        controls.LEFT = SDLK_a,
        controls.RIGHT = SDLK_f,
        controls.UP = SDLK_e,
        controls.DOWN = SDLK_d,
        controls.RETURN = SDLK_RETURN,
        controls.BACKSPACE = SDLK_BACKSPACE,
        controls.DELETE = SDLK_DELETE,

        controls.BUFFER_INC = SDLK_l,
        controls.BUFFER_DEC = SDLK_h,

        controls.ACTION_CMD = SDLK_c,
        controls.ACTION_MOD = KMOD_LCTRL,
        controls.META_MOD = KMOD_LALT,
    };
    return c;
}

//https://en.cppreference.com/w/cpp/language/lambda
void KeyEvent::ev_init_keybinds(void)
{
    binds.insert({controls.LEFT, [this](int mod, Editor *e, const int32_t id) -> EventResult { return prev_char(mod, e, id); }});
    binds.insert({controls.RIGHT, [this](int mod, Editor *e, const int32_t id) -> EventResult { return next_char(mod, e, id); }});
    binds.insert({controls.UP, [this](int mod, Editor *e, const int32_t id) -> EventResult { return prev_line(mod, e, id); }});
    binds.insert({controls.DOWN, [this](int mod, Editor *e, const int32_t id) -> EventResult { return next_line(mod, e, id); }});

    binds.insert({controls.BUFFER_DEC, [this](int mod, Editor *e, const int32_t id) -> EventResult { return prev_buffer(mod, e, id); }});
    binds.insert({controls.BUFFER_INC, [this](int mod, Editor *e, const int32_t id) -> EventResult { return next_buffer(mod, e, id); }});

    binds.insert({controls.RETURN, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_return(mod, e, id); }});
    binds.insert({controls.BACKSPACE, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_backspace(mod, e, id); }});
    binds.insert({controls.DELETE, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_delete(mod, e, id); }});

    binds.insert({controls.ACTION_CMD, [this](int mod, Editor *e, const int32_t id) -> EventResult { return ev_cmd_mode(mod, e, id); }});
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
    const size_t len = strlen(text);
    char input_buffer[len + 1];

    strncpy(input_buffer, text, sizeof(char) * len + 1);
    input_buffer[len] = '\0';

    switch(e->ed_taking_cmd()){
        default:{
            return EventResult(NO_KEY, NO_OPTION, id);
        }break;

        case 0: {
            e->ed_ins_char(id, input_buffer, len);
            return EventResult(BUFFER_MUTATION, NO_OPTION, id);
        }break;

        case 1:{
            e->ed_cmd_ins(input_buffer, len);
            return EventResult(COMMAND_MUTATION, NO_OPTION, id);
        }break;
    }
}

EventResult KeyEvent::ev_cmd_mode(const int &keymod, Editor *e, const int32_t id){
    if(keymod & controls.ACTION_MOD){
        e->ed_set_cmd_mode(!e->ed_taking_cmd());
    }
    return EventResult(NO_KEY, NO_OPTION, id);
}



int KeyEvent::ev_mainloop_die(void) { return NO_RUN; }
