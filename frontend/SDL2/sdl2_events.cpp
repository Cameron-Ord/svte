#include <SDL2/SDL.h>


#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"
#include "../../include/SDL2/sdl2_st_enums.hpp"


int KeyEvent::ev_mainloop_poll_event_type(SDL_Event* const e){
    while(SDL_PollEvent(e)){
        return e->type;
    }
    return NIL;
}

int KeyEvent::ev_mainloop_keydown(const int keysym, const int keymod){}
int KeyEvent::ev_mainloop_window_event(void){}
int KeyEvent::ev_mainloop_text_input(void){}
int KeyEvent::ev_mainloop_die(void){ return NO_RUN; }

