#include <SDL2/SDL.h>
#include <iostream>

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"
#include "../../include/SDL2/sdl2_st_enums.hpp"

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

        controls.ACTION_CMD = SDLK_c,
        controls.ACTION_MOD = KMOD_LCTRL,
    };
    return c;
}

//https://en.cppreference.com/w/cpp/language/lambda
void KeyEvent::ev_init_keybinds(void){
    binds.insert({controls.LEFT, [this](int mod){ev_left(mod);}});
    binds.insert({controls.RIGHT, [this](int mod){ev_right(mod);}});
    binds.insert({controls.UP, [this](int mod){ev_up(mod);}});
    binds.insert({controls.DOWN, [this](int mod){ev_down(mod);}});
}

void KeyEvent::ev_left(const int& keymod){

}

void KeyEvent::ev_up(const int& keymod){
}

void KeyEvent::ev_down(const int& keymod){
}

void KeyEvent::ev_right(const int& keymod){
}

void KeyEvent::ev_mainloop_keydown(const int keysym, const int keymod){
    std::unordered_map<int, std::function<void(int)>>::const_iterator it = binds.find(keysym);
    if(it != binds.end()){
        it->second(keymod);
    }
}

int KeyEvent::ev_mainloop_poll_event_type(SDL_Event* const e){
    while(SDL_PollEvent(e)){
        return e->type;
    }
    return SDL2_NIL;
}

int KeyEvent::ev_mainloop_window_event(void){}
int KeyEvent::ev_mainloop_text_input(void){}
int KeyEvent::ev_mainloop_die(void){ return NO_RUN; }

