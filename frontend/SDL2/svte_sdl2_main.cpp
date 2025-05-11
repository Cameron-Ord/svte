#include <iostream>
#include <SDL2/SDL.h>

#include "../../include/SDL2/sdl2_context.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"
#include "../../include/SDL2/sdl2_window.hpp"
#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/SDL2/sdl2_st_enums.hpp"

int main(int argc, char **argv){
    SDL2_Context context;
    if(context.sdl2_get_err() != NIL) {
        std::cerr << "Failed to initialize SDL2 -> " << SDL_GetError() << std::endl;
        return 1;
    }

    Window window;
    if(window.win_get_err() != NIL){
        std::cerr << "Failed to create window -> " << SDL_GetError() << std::endl;
        return 1;
    }
    
    Renderer renderer(window.win_get_window(), window.win_width_ptr(), window.win_height_ptr());
    if(renderer.rndr_get_err() != NIL){
        std::cerr << "Failed to create renderer -> " << SDL_GetError() << std::endl;
        return 1;
    }

    KeyEvent ev_handle;

    const int tpf = (1000.0 / context.sdl2_get_fps());
    uint64_t frame_start;
    int frame_time;

    context.sdl2_set_text_input(STOP);
    context.sdl2_set_run_state(RUN);
    window.win_show_window();

    while(context.sdl2_get_run_state() != NO_RUN){
        frame_start = SDL_GetTicks64();
        renderer.rndr_fill_bg(30, 32, 48, 255);
        renderer.rndr_clear();

        SDL_Event e;
        switch(ev_handle.ev_mainloop_poll_event_type(&e)){
            case SDL_KEYDOWN:{

            }break;

            case SDL_WINDOWEVENT:{

            }break;

            case SDL_TEXTINPUT:{

            }break;

            case SDL_QUIT:{
                context.sdl2_set_run_state(ev_handle.ev_mainloop_die());
            }break;
        }


        frame_time = SDL_GetTicks64() - frame_start;
        if (tpf > frame_time) {
            SDL_Delay(tpf - frame_time);
        }

        renderer.rndr_present();
    }


    return 0;
}