#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../../include/SDL2/sdl2_context.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"

#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_sys.hpp"
#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_defines.hpp"

int main(int argc, char **argv)
{
    Editor ed;
    if (ed.ed_get_error() != CORE_NIL) {
        std::cerr << "Failed to initialize editor!" << std::endl;
        return 1;
    }

    FileSys sys;
    if(sys.sys_get_err() != FS_NIL) {
        std::cerr << "Failed to initialize filesys!" << std::endl;
        return 1;
    }

    std::string fn;
    if (argc > 1 && argc < 3) {
        fn = std::string(argv[1]);
    }

    SDL2_Initializer init;
    if (init.init_get_err() != SDL2_NIL) {
        std::cerr << "Failed to initialize SDL2 -> " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL2_Context context(&ed);
    if (context.sdl2_get_err() != SDL2_NIL) {
        std::cerr << "Failed to build contexts! -> " << SDL_GetError() << std::endl;
        return 1;
    }

    Window *window = context.sdl2_get_win();
    Renderer *renderer = context.sdl2_get_rend();
    KeyEvent *ev_handle = context.sdl2_get_keyevent();

    window->win_dft_partition(renderer->_vf().vec_row_block(), renderer->rndr_vpad());
    renderer->rndr_init_cmd_viewport(window->_wp());
    
    renderer->rndr_commit_buffer(
        ed.ed_append_buffer(sys.sys_new_file(fn)), 
        window->_wp()
    ); 


    const int tpf = (1000.0 / context.sdl2_get_fps());
    uint64_t frame_start;
    int frame_time;

    renderer->rndr_set_blendmode(SDL_BLENDMODE_BLEND);

    window->win_show_window();
    context.sdl2_input_chmode(EventResult("chsdl2textinput", "stop", SDL2_NIL));
    context.sdl2_set_run_state(RUN);

    while (context.sdl2_get_run_state() != NO_RUN) {
        frame_start = SDL_GetTicks64();
        renderer->rndr_set_colour(30, 32, 48, 255);
        renderer->rndr_clear();

        SDL_Event e;
        switch (ev_handle->ev_mainloop_poll_event_type(&e)) {
        case SDL_KEYDOWN:
        {
            const int keysym = e.key.keysym.sym;
            const int keymod = e.key.keysym.mod;
            context.sdl2_mainloop_event_branch(ev_handle->ev_mainloop_keydown(keysym, keymod, &ed));
        } break;

        case SDL_WINDOWEVENT:
        {
            context.sdl2_mainloop_event_branch(ev_handle->ev_mainloop_window_event_type(e.window.event));
        } break;

        case SDL_TEXTINPUT:
        {
            const char *t = e.text.text;
            context.sdl2_mainloop_event_branch(ev_handle->ev_mainloop_text_input(t, &ed, ed.ed_get_current_id()));
        } break;

        case SDL_QUIT:
        {
            context.sdl2_set_run_state(ev_handle->ev_mainloop_die());
        } break;
        }

        renderer->rndr_draw_id(ed.ed_get_current_id());
        renderer->rndr_draw_cmd().rndr_cmd_cursor();

        frame_time = SDL_GetTicks64() - frame_start;
        if (tpf > frame_time) {
            SDL_Delay(tpf - frame_time);
        }

        renderer->rndr_present();
    }

    return 0;
}