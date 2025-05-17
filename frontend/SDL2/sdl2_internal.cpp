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

SDL2_Context::SDL2_Context(void) : fps_target(60), error(SDL2_NIL), running(NO_RUN),
                                   win(), rend(win.win_get_window()), vfont(rend.rndr_get_renderer()), events()
{
    sdl2_set_err(win.win_get_err());
    sdl2_set_err(rend.rndr_get_err());
    sdl2_set_err(vfont.vec_get_err());
    sdl2_init_proxy_fncs();
}

SDL2_Context::~SDL2_Context(void)
{
}

class Window *SDL2_Context::sdl2_get_win(void)
{
    return &win;
}
class Renderer *SDL2_Context::sdl2_get_rend(void)
{
    return &rend;
}
class VectorFont *SDL2_Context::sdl2_get_vfont(void)
{
    return &vfont;
}
class KeyEvent *SDL2_Context::sdl2_get_keyevent(void)
{
    return &events;
}

void SDL2_Context::sdl2_init_proxy_fncs(void){
    branches.insert({"chsdl2textinput", [this](const class EventResult& er) -> void { sdl2_input_chmode(er); }});
    branches.insert({"move", [this](const class EventResult& er) -> void { sdl2_rndr_cursor_update(er); }});
    branches.insert({"textinsert", [this](const class EventResult& er) -> void { sdl2_rndr_cursor_update(er); }});
    branches.insert({"resized", [this](const class EventResult& er) -> void { sdl2_window_size_update(er); }});
    branches.insert({"sizechanged", [this](const class EventResult& er) -> void { sdl2_window_size_update(er); }});
}


void SDL2_Context::sdl2_window_size_update(const class EventResult& er){
    win.win_update_window_values();
    rend.rndr_update_viewports(win.win_width(), win.win_height());
    rend.rndr_update_offsets(vfont.vec_row_block(), vfont.vec_col_block());
}

void SDL2_Context::sdl2_rndr_cursor_update(const class EventResult& er){
    if(er.get_event_id() < 0){
        std::cerr << "Invalid ID passed in EventResult!" << std::endl;
        return;
    }

    rend.rndr_update_offsets_by_id(
        er.get_event_id(), 
        vfont.vec_row_block(), 
        vfont.vec_col_block()
    );
}

void SDL2_Context::sdl2_input_chmode(const class EventResult& er){
    if(er.get_opt() == "start"){
        SDL_StartTextInput();
    } else if(er.get_opt() == "stop"){
        SDL_StopTextInput();
    }
}

void SDL2_Context::sdl2_mainloop_event_branch(const EventResult& er){
    std::unordered_map<std::string, std::function<void(const class EventResult&)>>::iterator it;
    it = branches.find(er.get_type());
    if(it != branches.end()){
        it->second(er);
    }
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