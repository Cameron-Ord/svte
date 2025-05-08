#include "../inc/editor.hpp"
#include "../inc/defines.hpp"
#include "../inc/renderer.hpp"
#include "../inc/window.hpp"

#include <filesystem>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <ctime>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL2! -> " << SDL_GetError();
        return 1;
    }

    if (TTF_Init() < 0) {
        std::cerr << "Failed to initialize SDL2_ttf! -> " << SDL_GetError();
        return 1;
    }

    Window window;
    if (window.get_state() != WIN_STATE_OK) {
        return 1;
    }

    Renderer renderer(window.get_window(), window.get_width(), window.get_height());
    if (renderer.get_state() != RNDR_STATE_OK) {
        return 1;
    }

    Chars *ch = renderer.renderer_get_chars();
    if (!(ch->ch_set_font(ch->ch_open_font("dogicapixel.ttf", 16)))) {
        return 1;
    }

    if (!ch->ch_create_textures(renderer.get_renderer())) {
        return 1;
    }

    srand(time(NULL));
    std::error_code err;
    std::filesystem::path cwd = std::filesystem::current_path(err);
    if (err) {
        std::cerr << "Failed to retrieve current path -> " << err.message() << std::endl;
        return 0;
    }

    std::string cpath = cwd.string();
    std::string fn;

    if (argc > 1 && argc < 3) {
        fn = std::string(argv[1]);
    }

    std::cout << "CWD: " << cpath << std::endl;
    std::cout << "FN: " << fn << std::endl;

    Editor ed = Editor(cpath);
    if(ed.ed_append_buffer(fn) == BUF_STATE_VALID){
        renderer.renderer_create_buffer_viewport(ed.ed_get_curr_id());
    }

    SDL_ShowWindow(window.get_window());
    SDL_StopTextInput();

    const int tpf = (1000.0 / 30);
    uint64_t frame_start;
    int frame_time;

    int quit = 0;
    while (!quit) {
        frame_start = SDL_GetTicks64();
        renderer.renderer_fill_bg(0, 0, 0, 255);
        renderer.renderer_clear();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
        //Really need to find a nicer way to handle events cause this will get out of hand fast.
            switch (e.type) {
            default:
                break;

            case SDL_WINDOWEVENT:
            {
                switch (e.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    window.win_update_dimensions();
                    renderer.renderer_update_viewports(ed.ed_get_open());
                } break;
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    window.win_update_dimensions();
                    renderer.renderer_update_viewports(ed.ed_get_open());
                } break;
                }
            } break;
            case SDL_TEXTINPUT:
            {
                const char *t = e.text.text;
                const size_t tlen = strlen(t);
                for (size_t i = 0; i < tlen; i++) {
                    switch (ed.ed_get_mode()) {
                    default:
                        break;

                    case INSERT:
                    {
                        ed.ed_str_op(INS, t[i]);
                    } break;

                    case APPEND:
                    {
                        ed.ed_str_op(APND, t[i]);
                    } break;

                    case CMD:{
                        ed.ed_append_cmd(t[i]);
                    }break;
                    }
                }
            } break;
            case SDL_KEYDOWN:
            {
                const int sym = e.key.keysym.sym;
                const int mod = e.key.keysym.mod;
                switch (sym) {

                case SDLK_s:{
                    if(ed.ed_get_mode() == VISUAL && mod & KMOD_LCTRL){
                        ed.ed_save_buffer();
                    }
                }break;

                case SDLK_ESCAPE:
                {
                    ed.ed_set_mode(VISUAL);
                    SDL_StopTextInput();
                } break;

                case SDLK_a:
                {
                    if (ed.ed_get_mode() == VISUAL) {
                        ed.ed_set_mode(APPEND);
                        SDL_StartTextInput();
                    }
                } break;

                //insert mode
                case SDLK_i:
                {
                    if (ed.ed_get_mode() == VISUAL) {
                        ed.ed_set_mode(INSERT);
                        SDL_StartTextInput();
                    }
                } break;

                case SDLK_SPACE:{
                    if(ed.ed_get_mode() == VISUAL && mod & KMOD_SHIFT){
                        ed.ed_set_mode(CMD);
                        SDL_StartTextInput();
                    }
                }break;

                case SDLK_TAB:
                {
                    //     editor.ins(&editor, SPACECHAR);
                } break;

                case SDLK_BACKSPACE:
                {
                    ed.ed_str_op(RMV, NULLCHAR);
                } break;

                case SDLK_DELETE:
                {
                    switch(ed.ed_get_mode()){
                        case CMD:{
                            ed.ed_del_cmd();
                        }break;

                        default:{
                            ed.ed_str_op(DEL, NULLCHAR);
                        }break;
                    }
                } break;

                case SDLK_RETURN:
                {
                    switch(ed.ed_get_mode()){
                        case CMD:{
                            if(ed.ed_enter_cmd() == CMD_STATE_OK){
                                renderer.renderer_create_buffer_viewport(ed.ed_get_curr_id());
                            }
                            ed.ed_set_mode(VISUAL);
                        }break;

                        default: {
                            ed.ed_str_op(C_NEWLINE, NEWLINE);
                        }break;
                    }
                } break;

                case SDLK_h:
                {
                    switch (ed.ed_get_mode()) {
                    default:
                        break;
                    case VISUAL:
                    {
                        ed.ed_mv_op(MV_LEFT);
                    } break;
                    }
                } break;

                case SDLK_j:
                {
                    switch (ed.ed_get_mode()) {
                    default:
                        break;
                    case VISUAL:
                    {
                        ed.ed_mv_op(NEXT_LINE);
                    } break;
                    }
                } break;

                case SDLK_k:
                {
                    switch (ed.ed_get_mode()) {
                    default:
                        break;
                    case VISUAL:
                    {
                        ed.ed_mv_op(PREV_LINE);
                    } break;
                    }
                } break;

                case SDLK_l:
                {
                    switch (ed.ed_get_mode()) {
                    default:
                        break;
                    case VISUAL:
                    {
                        if(mod & KMOD_LCTRL){
                            ed.ed_change_buffer(NEXT_BUFFER);
                        } else {
                            ed.ed_mv_op(MV_RIGHT);
                        }
                    } break;
                    }
                } break;
                }
            } break;
            case SDL_KEYUP:
            {
            } break;
            case SDL_QUIT:
            {
                quit = 1;
            } break;
            }
        }

        Buffer *buf = ed.ed_grab_buffer();
        renderer.renderer_draw_file(buf);
        renderer.renderer_draw_cursor(*buf->buf_get_row(), *buf->buf_get_col());
        frame_time = SDL_GetTicks64() - frame_start;
        if (tpf > frame_time) {
            SDL_Delay(tpf - frame_time);
        }

        renderer.renderer_present();
    }

    SDL_DestroyRenderer(renderer.get_renderer());
    SDL_DestroyWindow(window.get_window());
    //TTF_CloseFont(font.get_font()->def);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
