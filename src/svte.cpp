#include "../inc/editor.hpp"
#include "../inc/font.hpp"
#include "../inc/renderer.hpp"
#include "../inc/window.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstdlib>
#include <filesystem>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL2! -> %s\n", SDL_GetError());
    return 1;
  }

  if (TTF_Init() < 0) {
    fprintf(stderr, "Failed to initialize SDL2_ttf! -> %s\n", TTF_GetError());
    return 1;
  }

  std::string fn = "";
  if (argc > 1 && argc < 3) {
    int i = 0;
    const char *farg = argv[1];
    while (farg[i] != '\0') {
      fn += farg[i];
      i++;
    }
  }

  Editor editor(std::filesystem::current_path().string(), fn);

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
  if (!(font.get_font()->def = font.open_font("dogicapixel.ttf", 8))) {
    return 1;
  }

  if (!(font.get_font()->title = font.open_font("dogicapixel.ttf", 48))) {
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
