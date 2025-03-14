#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/window.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <linux/limits.h>
#include <unistd.h>

#ifndef NEWLINE
#define NEWLINE '\n'
#endif

#ifndef SPACECHAR
#define SPACECHAR ' '
#endif

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL2! -> %s\n", SDL_GetError());
    return 1;
  }

  if (TTF_Init() < 0) {
    fprintf(stderr, "Failed to initialize SDL2_ttf! -> %s\n", TTF_GetError());
    return 1;
  }

  char *arg_str = NULL;
  if (argc > 1 && argc < 3) {
    arg_str = strdup(argv[1]);
  }

  char cwd[PATH_MAX];
  if (!getcwd(cwd, PATH_MAX)) {
    return 1;
  }

  Editor editor(cwd, arg_str);

  Window window;
  if (!window.create_window()) {
    return 1;
  }

  Renderer renderer(&window);
  if (!renderer.create_renderer(window.get_window())) {
    return 1;
  }
  renderer._frender()->frender_set_renderer(renderer.get_renderer());

  Font font;
  if (!(font.get_font()->def = font.open_font("dogicapixel.ttf", 16))) {
    return 1;
  }

  if (!font._chars()->table_create_textures(renderer.get_renderer(),
                                            font.get_font())) {
    return 1;
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
      case SDL_TEXTINPUT: {
        const char *t = e.text.text;
        const size_t tlen = strlen(t);
        for (size_t i = 0; i < tlen; i++) {
          editor.buffer_insert_char(t[i]);
        }
      } break;
      case SDL_KEYDOWN: {
        const int sym = e.key.keysym.sym;
        const int mod = e.key.keysym.mod;
        switch (sym) {

        case SDLK_TAB: {
          editor.buffer_insert_char(SPACECHAR);
        } break;

        case SDLK_BACKSPACE: {
          editor.buffer_rm_char();
        } break;

        case SDLK_DELETE: {
          editor.buffer_del_char();
        } break;

        case SDLK_RETURN: {
          editor.buffer_insert_char(NEWLINE);
        } break;

        case SDLK_LEFT: {
          if (mod & KMOD_SHIFT) {
            editor.buffer_mv_op(PREV_WORD);
          } else {
            editor.buffer_mv_op(MV_LEFT);
          }
        } break;

        case SDLK_RIGHT: {
          if (mod & KMOD_SHIFT) {
            editor.buffer_mv_op(NEXT_WORD);
          } else {
            editor.buffer_mv_op(MV_RIGHT);
          }
        } break;
        case SDLK_UP: {
          editor.buffer_mv_op(PREV_LINE);
        } break;
        case SDLK_DOWN: {
          editor.buffer_mv_op(NEXT_LINE);
        } break;
        }
      } break;
      case SDL_KEYUP: {
      } break;
      case SDL_QUIT: {
        quit = 1;
      } break;
      }
    }

    renderer._frender()->render_buffer(editor._bufs()->get_buf(0),
                                       font._chars());
    renderer._frender()->render_curs(editor._bufs()->get_buf(0),
                                     font._chars()->get_char_dims());

    frame_time = SDL_GetTicks64() - frame_start;
    if (tpf > frame_time) {
      SDL_Delay(tpf - frame_time);
    }

    renderer.render_present();
  }

  SDL_DestroyRenderer(renderer.get_renderer());
  SDL_DestroyWindow(window.get_window());
  TTF_CloseFont(font.get_font()->title);
  TTF_CloseFont(font.get_font()->def);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
