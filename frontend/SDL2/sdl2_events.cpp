#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"
#include "../../include/SDL2/sdl2_st_enums.hpp"


#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"

KeyEvent::KeyEvent(void) : controls(ev_init_controls()) {
    ev_init_keybinds();
    std::cout << "KeyEvents initialized" << std::endl;
}

Controls KeyEvent::ev_init_controls(void){
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
void KeyEvent::ev_init_keybinds(void){
    binds.insert({controls.LEFT, [this](int mod, class Editor *e) -> int { return ev_left(mod, e);}});
    binds.insert({controls.RIGHT, [this](int mod, class Editor *e) -> int { return ev_right(mod, e);}});
    binds.insert({controls.UP, [this](int mod, class Editor *e) -> int { return ev_up(mod, e);}});
    binds.insert({controls.DOWN, [this](int mod, class Editor *e) -> int { return ev_down(mod, e);}});
    binds.insert({controls.INSERT_MODE, [this](int mod, class Editor *e) -> int { return ev_insert(mod, e);}});
    binds.insert({controls.APPEND_MODE, [this](int mod, class Editor *e) -> int { return ev_append(mod, e);}});
    binds.insert({controls.ESCAPE, [this](int mod, class Editor *e) -> int { return ev_escape(mod, e);}});
    binds.insert({controls.VISUAL_MODE, [this](int mod, class Editor *e) -> int { return ev_visual(mod, e);}});
}

int KeyEvent::ev_left(const int& keymod, class Editor *e){
    if(e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT){

    }
    return SDL2_NIL;
}

int KeyEvent::ev_up(const int& keymod, class Editor *e){
    if(e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT){
        
    }
    return SDL2_NIL;
}

int KeyEvent::ev_down(const int& keymod, class Editor *e){
    if(e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT){
        
    }
    return SDL2_NIL;
}

int KeyEvent::ev_right(const int& keymod, class Editor *e){
    if(e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT){
        
    }
    return SDL2_NIL;
}


int KeyEvent::ev_visual(const int& keymod, class Editor *e){
    if(e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT){
        e->ed_set_mode(VISUAL);
    }
    return SDL2_NIL;
}

int KeyEvent::ev_append(const int& keymod, class Editor *e){
    if(e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT){
        e->ed_set_mode(APPEND);
        e->ed_set_edit(EDIT);
        return START_INPUT;
    }
    return SDL2_NIL;
}

int KeyEvent::ev_insert(const int& keymod, class Editor *e){
    if(e->ed_get_mode() == NVISUAL && e->ed_get_edit() == NO_EDIT){
        e->ed_set_mode(INSERT);
        e->ed_set_edit(EDIT);
        return START_INPUT;
    }
    return SDL2_NIL;
}

int KeyEvent::ev_escape(const int& keymod, class Editor *e){
    e->ed_set_mode(NVISUAL);
    e->ed_set_edit(NO_EDIT);
    return STOP_INPUT;
}

int KeyEvent::ev_mainloop_keydown(const int keysym, const int keymod, class Editor *e){
    std::unordered_map<int, std::function<int(int, class Editor *e)>>::const_iterator it = binds.find(keysym);
    if(it != binds.end()){
        return it->second(keymod, e);
    }
    return SDL2_NIL;
}

int KeyEvent::ev_mainloop_poll_event_type(SDL_Event* const e){
    while(SDL_PollEvent(e)){
        return e->type;
    }
    return SDL2_NIL;
}

int KeyEvent::ev_mainloop_window_event(void){}

void KeyEvent::ev_mainloop_text_input(const char *text, class Editor *e){
    const size_t len = strlen(text);
    for(size_t i = 0; i < len; i++){
        e->ed_ins_char(text[i]);
    }
}

int KeyEvent::ev_mainloop_die(void){ return NO_RUN; }

