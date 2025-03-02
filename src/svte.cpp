#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstdlib>

#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/window.hpp"

typedef enum { MENU = 0, EDITOR = 1 } STATES;

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL2! -> %s\n", SDL_GetError());
    return 1;
  }

  if (TTF_Init() < 0) {
    fprintf(stderr, "Failed to initialize SDL2_ttf! -> %s\n", TTF_GetError());
    return 1;
  }

  Window *win = get_window_inst();
  if (!win->create_window()) {
    return 1;
  }

  Renderer *rend = get_renderer_inst();
  if (!rend->create_renderer(win->get_window())) {
    return 1;
  }

  Font *font = get_font_inst();
  if (!(font->get_font()->def = font->open_font("dogicapixel.ttf", 8))) {
    return 1;
  }

  if (!(font->get_font()->title = font->open_font("dogicapixel.ttf", 48))) {
    return 1;
  }

  if (!font->table_create_textures(rend->get_renderer())) {
    return 1;
  }

  if (!font->table_create_textures(rend->get_renderer())) {
    return 1;
  }

  SDL_ShowWindow(win->get_window());
  SDL_StopTextInput();

  const int tpf = (1000.0 / 30);
  uint64_t frame_start;
  int frame_time;

  const int MODE = MENU;

  int quit = 0;
  while (!quit) {
    frame_start = SDL_GetTicks64();
    rend->bg_fill(0, 0, 0, 255);
    rend->clear_render();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      default:
        break;
      case SDL_TEXTINPUT: {
      } break;
      case SDL_KEYDOWN: {
      } break;
      case SDL_KEYUP: {
      } break;
      case SDL_QUIT: {
        quit = 1;
      } break;
      }
    }

    switch (MODE) {
    default:
      break;
    case MENU: {
      rend->render_logo("SVTE", 4);
    } break;
    case EDITOR: {
    } break;
    }

    frame_time = SDL_GetTicks64() - frame_start;
    if (tpf > frame_time) {
      SDL_Delay(tpf - frame_time);
    }

    rend->render_present();
  }

  SDL_DestroyRenderer(rend->get_renderer());
  SDL_DestroyWindow(win->get_window());
  TTF_CloseFont(font->get_font()->title);
  TTF_CloseFont(font->get_font()->def);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
