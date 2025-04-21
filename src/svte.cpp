#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/window.hpp"
#include "../inc/globaldef.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <linux/limits.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL2! -> %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Failed to initialize SDL2_ttf! -> %s\n", TTF_GetError());
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
    if (!(ch.set_font(ch.open_font("dogicapixel.ttf", 16)))) {
        return 1;
    }

    if (!ch.table_create_textures(renderer.get_renderer(),
                                              ch.get_fonts())) { return 1;
    }


    char *filename_arg = NULL;
    if (argc > 1 && argc < 3) {
        filename_arg = strdup(argv[1]);
    }

    char cwd[PATH_MAX];
    if (!getcwd(cwd, PATH_MAX)) {
        std::cerr << "Subpath could not be determined!" << std::endl;
        return 1;
    }
    
    Editor ed = Editor();
    ed.ed_append_buffer(filename_arg, cwd);
    if(filename_arg){
        free(filename_arg);
    }

    SDL_ShowWindow(window.get_window());
    SDL_StartTextInput();

    const int tpf = (1000.0 / 30);
    uint64_t frame_start;
    int frame_time;

    int quit = 0;
    while (!quit) {
        frame_start = SDL_GetTicks64();
        renderer.bg_fill(0, 0, 0, 255);
        renderer.clear_render();

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
                   // editor.ins(&editor, t[i]);
                }
            } break;
            case SDL_KEYDOWN:
            {
                const int sym = e.key.keysym.sym;
                const int mod = e.key.keysym.mod;
                switch (sym) {

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

     //    renderer.render_buffer(buffers.get_buf(editor.current_buffer), font._chars(), window.get_dimensions());
     //   renderer.render_curs(buffers.get_buf(editor.current_buffer), window.get_dimensions());

        frame_time = SDL_GetTicks64() - frame_start;
        if (tpf > frame_time) {
            SDL_Delay(tpf - frame_time);
        }

        renderer.render_present();
    }

    SDL_DestroyRenderer(renderer.get_renderer());
    SDL_DestroyWindow(window.get_window());
    TTF_CloseFont(font.get_font()->def);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
