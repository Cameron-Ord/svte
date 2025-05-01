#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/window.hpp"

#include <iostream>
#include <filesystem>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <linux/limits.h>
#include <unistd.h>
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
    if (!window.create_window()) {
        return 1;
    }

    Renderer renderer;
    if (!renderer.create_renderer(window.get_window())) {
        return 1;
    }

    //This is going to have to be reworked to have like 12 different fonts but
    //thats also when I have to write syntax highlighting so just keep it simple
    //for now.
    Chars ch;
    if (!(ch.ch_set_font(ch.ch_open_font("dogicapixel.ttf", 16)))) {
        return 1;
    }

    if (!ch.ch_create_textures(renderer.get_renderer(),
                                              ch.ch_get_fonts())) {
        return 1;
    }

    srand(time(NULL));
    std::error_code err;
    std::filesystem::path cwd = std::filesystem::current_path(err);
    if(err){
        std::cerr << "Failed to retrieve current path -> " << err.message() << std::endl;
        return 0;
    }

    size_t arg_slen = 0;
    if (argc > 1 && argc < 3) {
        arg_slen = strlen(argv[1]);
    }

    std::string cpath = cwd.string();
    std::string fn(argv[1], arg_slen);

    std::cout << "CWD: " << cpath << std::endl;
    std::cout << "FN: " << fn << std::endl;

    Editor ed = Editor();
    ed.ed_append_buffer(fn, cpath);

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
            switch (e.type) {
            default:
                break;

            case SDL_WINDOWEVENT:{
                switch(e.window.event){
                    case SDL_WINDOWEVENT_RESIZED:{
                        window.win_update_dimensions();
                    }break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:{
                        window.win_update_dimensions();
                    }break;
                }
            }break;
            case SDL_TEXTINPUT:
            {
                const char *t = e.text.text;
                const size_t tlen = strlen(t);
                for (size_t i = 0; i < tlen; i++) {
                   ed.ed_ins_char(t[i]);
                }
            } break;
            case SDL_KEYDOWN:
            {
                const int sym = e.key.keysym.sym;
                const int mod = e.key.keysym.mod;
                switch (sym) {
                
                case SDLK_ESCAPE:{
                    ed.ed_set_mode(VISUAL);
                    SDL_StopTextInput();
                }break;

                //insert mode
                case SDLK_i:{
                    ed.ed_set_mode(INSERT);
                    SDL_StartTextInput();
                }break;

                case SDLK_TAB:
                {
                //     editor.ins(&editor, SPACECHAR);
                } break;

                case SDLK_BACKSPACE:
                {
                //    editor.rm(&editor);
                } break;

                case SDLK_DELETE:
                {
                //    editor.del(&editor);
                } break;

                case SDLK_RETURN:
                {
                //    editor.ins(&editor, NEWLINE);
                } break;

                case SDLK_h:
                {
                    switch(ed.ed_get_mode()){
                        default: break;
                        case VISUAL: {
                            ed.ed_mv_op(MV_LEFT);
                         }break;
                        case INSERT: {

                        }break;
                        case REPLACE: {

                        }break;
                    }
                } break;

                case SDLK_j:
                {
                    switch(ed.ed_get_mode()){
                        default: break;
                        case VISUAL: {
                            ed.ed_mv_op(NEXT_LINE);
                         }break;
                        case INSERT: {

                        }break;
                        case REPLACE: {

                        }break;
                    }
                } break;

                case SDLK_k:
                {
                    switch(ed.ed_get_mode()){
                        default: break;
                        case VISUAL: {
                            ed.ed_mv_op(PREV_LINE);
                         }break;
                        case INSERT: {

                        }break;
                        case REPLACE: {

                        }break;
                    }
                } break;

                case SDLK_l:
                {
                    switch(ed.ed_get_mode()){
                        default: break;
                        case VISUAL: {
                            ed.ed_mv_op(MV_RIGHT);
                         }break;
                        case INSERT: {

                        }break;
                        case REPLACE: {

                        }break;
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
        renderer.renderer_draw_file(ed.ed_grab_buffer(), &ch);
        renderer.renderer_draw_cursor(ed.ed_grab_buffer()->buf_get_curs(), ch.ch_max_width(), ch.ch_max_height());
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
