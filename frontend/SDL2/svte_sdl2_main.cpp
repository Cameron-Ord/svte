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

static void event_chain_update(
    const EventResult er, SDL2_Context *ctx, Editor *e, 
    FileSys *sys, const Buffer*& cbptr, const RndrItem*& citemptr
);

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
    
    renderer->rndr_commit_buffer(
        ed.ed_append_buffer(sys.sys_new_file(fn)), 
        window->_wp()
    ); 
    renderer->rndr_update_viewports(window->_wp());

    const Buffer *b = ed.ed_fetch_buffer(ed.ed_get_current_id());
    const RndrItem *item = renderer->rndr_get_buffer_item(ed.ed_get_current_id());

    const int tpf = (1000.0 / context.sdl2_get_fps());
    uint64_t frame_start;
    int frame_time;

    SDL_StartTextInput();
    renderer->rndr_set_blendmode(SDL_BLENDMODE_BLEND);
    window->win_show_window();
    context.sdl2_set_run_state(RUN);

    while (context.sdl2_get_run_state() != NO_RUN) {
        frame_start = SDL_GetTicks64();
        renderer->rndr_set_colour(40, 42, 54, 255);
        renderer->rndr_clear();

        SDL_Event e;
        switch (ev_handle->ev_mainloop_poll_event_type(&e)) {
        case SDL_KEYDOWN:
        {
            const int keysym = e.key.keysym.sym;
            const int keymod = e.key.keysym.mod;
            event_chain_update(
                ev_handle->ev_mainloop_keydown(keysym, keymod, &ed),
                &context, &ed, &sys, b, item
            );
        } break;

        case SDL_WINDOWEVENT:
        {
            event_chain_update(
                ev_handle->ev_mainloop_window_event_type(e.window.event),
                &context, &ed, &sys, b, item
            );
        } break;

        case SDL_TEXTINPUT:
        {
            const char *t = e.text.text;
            event_chain_update(
                ev_handle->ev_mainloop_text_input(t, &ed, ed.ed_get_current_id()),
                &context, &ed, &sys, b, item
            );
        } break;

        case SDL_QUIT:
        {
            context.sdl2_set_run_state(ev_handle->ev_mainloop_die());
        } break;
        }

        renderer->rndr_set_viewport(&item->viewport).rndr_draw_buffer(item, b);
        renderer->rndr_set_colour(189, 147, 249, 200).rndr_put_cursor(
            item, b->buf_get_row(), b->buf_get_col()
        );

        renderer->rndr_set_viewport(
            &renderer->rndr_get_filename()->viewport
        ).rndr_draw_filename(b->buf_get_filename());

        renderer->rndr_set_viewport(
            &renderer->rndr_get_rcmd()->viewport
        ).rndr_set_colour(189, 147, 249, 200).rndr_draw_cmd().rndr_cmd_cursor();


        frame_time = SDL_GetTicks64() - frame_start;
        if (tpf > frame_time) {
            SDL_Delay(tpf - frame_time);
        }

        renderer->rndr_present();
    }

    return 0;
}

static void event_chain_update(
    const EventResult er, SDL2_Context *ctx, Editor *e, FileSys *sys,
    const Buffer*& cb_ptr, const RndrItem*& citem_ptr
){
    const int32_t id = er.get_event_id();
    const uint8_t key = er.get_key();
    const uint8_t opt = er.get_opt();

    switch(key){
        case BUFFER_CHANGED:{
            //Overwrite the old const ptrs with new ones.
            if(cb_ptr && citem_ptr){
                cb_ptr = e->ed_fetch_buffer(e->ed_get_current_id());
                citem_ptr = ctx->sdl2_get_rend()->rndr_get_buffer_item(e->ed_get_current_id());
            }
        }break;

        case DISPLAY_SIZE_CHANGED:{
            Window *w = ctx->sdl2_get_win();
            Renderer *r = ctx->sdl2_get_rend();

            w->win_update_window_values().win_dft_partition(
                r->_vf().vec_row_block(), r->rndr_vpad()
            );
            r->rndr_update_viewports(w->_wp()).rndr_update_offsets().rndr_cmd_offsets();
        }break;

        case COMMAND_MUTATION:{
            Renderer *r = ctx->sdl2_get_rend();
            r->rndr_cmd_offsets();
        }break;

        case BUFFER_MUTATION:{
            Renderer *r = ctx->sdl2_get_rend();
            r->rndr_update_offsets_by_id(id);

            Buffer *buf = e->ed_fetch_buffer(id);
            if(buf){
                buf->buf_tokenize();
            }
        }break;

        case COMMAND_COMMIT:{
            switch(opt){
                default: break;

                case NEW_BUFFER:{
                    Renderer *r = ctx->sdl2_get_rend();
                    Window *w = ctx->sdl2_get_win();

                    r->rndr_commit_buffer(
                        e->ed_append_buffer(sys->sys_new_file(e->ed_get_cmd().sanitized)), 
                        w->_wp()
                    );  
                    r->rndr_update_viewports(w->_wp());

                    if(cb_ptr && citem_ptr){
                        cb_ptr = e->ed_fetch_buffer(e->ed_get_current_id());
                        citem_ptr = ctx->sdl2_get_rend()->rndr_get_buffer_item(e->ed_get_current_id());
                    }
                }break;
            }
        }break;

        case COMMAND_CURSOR_MOVE : {
            Renderer *r = ctx->sdl2_get_rend();
            r->rndr_cmd_offsets();
        }break;

        case BUFFER_CURSOR_MOVE: {
            Renderer *r = ctx->sdl2_get_rend();
            r->rndr_update_offsets_by_id(id);
        }break;

    }



}
