#include <iostream>
#include <SDL2/SDL.h>

#include "../../include/SDL2/sdl2_context.hpp"
#include "../../include/SDL2/sdl2_window.hpp"
#include "../../include/SDL2/sdl2_renderer.hpp"
#include "../../include/SDL2/sdl2_events.hpp"
#include "../../include/SDL2/sdl2_enums.hpp"


#include "../../include/core/core_editor.hpp"
#include "../../include/core/core_buffer.hpp"
#include "../../include/core/core_error_codes.hpp"


int main(int argc, char **argv){
    SDL2_Context context;
    if(context.sdl2_get_err() != SDL2_NIL) {
        std::cerr << "Failed to initialize SDL2 -> " << SDL_GetError() << std::endl;
        return 1;
    }

    Window window;
    if(window.win_get_err() != SDL2_NIL){
        std::cerr << "Failed to construct window -> " << SDL_GetError() << std::endl;
        return 1;
    }
    
    Renderer renderer(window.win_get_window(), window.win_width_ptr(), window.win_height_ptr());
    if(renderer.rndr_get_err() != SDL2_NIL){
        std::cerr << "Failed to construct renderer -> " << SDL_GetError() << std::endl;
        return 1;
    }

    Editor ed;
    if(ed.ed_get_error() != CORE_NIL){
        std::cerr << "Failed to initialize editor!" << std::endl;
        return 1;
    }

    std::string fn;
    if (argc > 1 && argc < 3) {
        fn = std::string(argv[1]);
    }

    const int cond = !fn.empty();
    switch(cond){
        case 1:{
            const int id = ed.ed_append_buffer(fn);
            if(id != ED_BAD_APPEND && renderer.rndr_insert_pos(id)){
                ed.ed_open_file(id);
            } else {
                std::cerr << "Buffer was not properly created!" << std::endl;
            }
        }break;

        case 0:{
            const int id = ed.ed_append_buffer();
            if(!(id != ED_BAD_APPEND && renderer.rndr_insert_pos(id))){
                std::cerr << "Buffer was not properly created!" << std::endl;
            }
        }break;
    }

    const int tpf = (1000.0 / context.sdl2_get_fps());
    uint64_t frame_start;
    int frame_time;

    context.sdl2_set_text_input(STOP_INPUT);
    context.sdl2_set_run_state(RUN);
    window.win_show_window();

    KeyEvent ev_handle;

    while(context.sdl2_get_run_state() != NO_RUN){
        frame_start = SDL_GetTicks64();
        renderer.rndr_fill_bg(30, 32, 48, 255);
        renderer.rndr_clear();

        SDL_Event e;
        switch(ev_handle.ev_mainloop_poll_event_type(&e)){
            case SDL_KEYDOWN:{
                const EventRet ret = ev_handle.ev_mainloop_keydown(e.key.keysym.sym, e.key.keysym.mod, &ed, ed.ed_get_current_id());
                renderer.rndr_eval_pos(ret.id, ret.reported_cursor_loc, ret.reported_row_loc);
                context.sdl2_set_text_input(ret.input_opt);
            }break;

            case SDL_WINDOWEVENT:{
                    window.win_check_size_update(ev_handle.ev_mainloop_window_event_type(e.window.event));
            }break;

            case SDL_TEXTINPUT:{
                ev_handle.ev_mainloop_text_input(e.text.text, &ed, ed.ed_get_current_id());
            }break;

            case SDL_QUIT:{
                context.sdl2_set_run_state(ev_handle.ev_mainloop_die());
            }break;
        }

        //At some point multiple buffers at once would be cool but lets just focus on other things first.
        class Buffer *b = nullptr;
        if((b = ed.ed_fetch_buffer(ed.ed_get_current_id())) != nullptr){
            renderer.rndr_do_pipeline(
                b->buf_row_begin_const(),
                b->buf_row_end_const(),
                b->buf_get_row(), 
                b->buf_get_col()
            );
        }


        frame_time = SDL_GetTicks64() - frame_start;
        if (tpf > frame_time) {
            SDL_Delay(tpf - frame_time);
        }

        renderer.rndr_present();
    }


    return 0;
}