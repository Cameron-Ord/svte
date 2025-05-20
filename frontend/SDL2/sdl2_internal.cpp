#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "../../include/SDL2/sdl2_context.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"
#include "../../include/SDL2/sdl2_macdef.hpp"

#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_window.hpp"

SDL2_Initializer::SDL2_Initializer(void) : error(SDL2_NIL)
{
    init_set_err(init_sdl2_instance());
    init_set_err(init_open_ttf());
}

int SDL2_Initializer::init_get_err(void)
{
    return error;
}

void SDL2_Initializer::init_set_err(const int err)
{
    error = err;
}

int SDL2_Initializer::init_open_ttf(void)
{
    if (TTF_Init() < 0) {
        return SDL2_ERR;
    }
    return SDL2_NIL;
}

int SDL2_Initializer::init_sdl2_instance(void)
{
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        return SDL2_ERR;
    }
    return SDL2_NIL;
}

SDL2_Context::SDL2_Context(const Editor *const edptr) : fps_target(60), error(SDL2_NIL), running(NO_RUN),
                                                        win(), rend(win.win_get_window(), edptr), events()
{
    sdl2_set_err(win.win_get_err());
    sdl2_set_err(rend.rndr_get_err());
    sdl2_set_err(rend._vf().vec_get_err());
}

SDL2_Context::~SDL2_Context(void)
{
}

int SDL2_Context::sdl2_get_fps(void)
{
    return fps_target;
}

int SDL2_Context::sdl2_get_err(void)
{
    return error;
}

void SDL2_Context::sdl2_set_err(const int err)
{
    error = err;
}

void SDL2_Context::sdl2_set_run_state(int sval)
{
    running = sval;
}

int SDL2_Context::sdl2_get_run_state(void)
{
    return running;
}