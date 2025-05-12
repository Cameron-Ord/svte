#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "../../include/SDL2/sdl2_context.hpp"
#include "../../include/SDL2/sdl2_error_codes.hpp"
#include "../../include/SDL2/sdl2_st_enums.hpp"


SDL2_Context::SDL2_Context(void) : fps_target(60), error(SDL2_NIL), running(NO_RUN) {
    sdl2_set_err(sdl2_open_instance());
    sdl2_set_err(sdl2_open_ttf());
}

SDL2_Context::~SDL2_Context(void){

}

void SDL2_Context::sdl2_set_text_input(const int opt){
    switch(opt){
        default: break;
        case STOP_INPUT:{
            SDL_StopTextInput();
            std::cout << "Input stopped!" << std::endl;
        }break;
    
        case START_INPUT:{
            SDL_StartTextInput();
            std::cout << "Input started!" << std::endl;
        }break;
    }
}


int SDL2_Context::sdl2_get_fps(void){
    return fps_target;
}

int SDL2_Context::sdl2_open_ttf(void){
    if(TTF_Init() < 0){
        return BAD_INIT;
    }
    return SDL2_NIL;
}

int SDL2_Context::sdl2_open_instance(void){
    if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
        return BAD_INIT;
    }
    return SDL2_NIL;
}

int SDL2_Context::sdl2_get_err(void){
    return error;
}

void SDL2_Context::sdl2_set_err(const int err){
    error = err;
}

void SDL2_Context::sdl2_set_run_state(int sval){
    running = sval;
}

int SDL2_Context::sdl2_get_run_state(void){
    return running;
}

void SDL2_Context::sdl2_quit_instance(void){
    SDL_Quit();
}
