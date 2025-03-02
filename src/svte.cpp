#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstdlib>

#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/window.hpp"

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
  if (!font->open_font("dogicapixel.ttf")) {
    return 1;
  }

  if (!font->table_create_textures(rend->get_renderer())) {
    return 1;
  }

  SDL_ShowWindow(win->get_window());

  const int tpf = (1000.0 / 30);
  uint64_t frame_start;
  int frame_time;

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

      case SDL_KEYDOWN: {
      } break;

      case SDL_KEYUP: {
      } break;

      case SDL_QUIT: {
        quit = 1;
      } break;
      }
    }

    frame_time = SDL_GetTicks64() - frame_start;
    if (tpf > frame_time) {
      SDL_Delay(tpf - frame_time);
    }

    rend->render_present();
  }

  SDL_DestroyRenderer(rend->get_renderer());
  SDL_DestroyWindow(win->get_window());
  TTF_CloseFont(font->get_font());
  TTF_Quit();
  SDL_Quit();

  return 0;
}
