#include "../inc/renderer.hpp"
#include "../inc/font.hpp"
#include "../inc/window.hpp"

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <SDL2/SDL_render.h>

Renderer renderer_instance;

Renderer::~Renderer(void) {}

void Renderer::render_logo(const char *str, const size_t len) {
  Vec2i dim = get_window_inst()->get_dimensions();
  Font *f = get_font_inst();

  std::vector<SDL_Rect> rect(len);
  std::vector<Char_Tables *> ch(len);

  int xpos = dim.x * 0.5, ypos = dim.y * 0.25;
  const int padding = 2;
  int total_width = 0;

  for (size_t i = 0; i < len; i++) {
    ch[i] = f->char_lookup(str[i], TITLE);
    total_width += ch[i]->title.width + padding;
  }

  for (size_t i = 0; i < len; i++) {
    const int set_xpos = xpos - (total_width * 0.5);
    rect[i].x = set_xpos, rect[i].y = ypos, rect[i].w = ch[i]->title.width,
    rect[i].h = ch[i]->title.height;
    SDL_RenderCopy(rend, ch[i]->title.t, NULL, &rect[i]);
    xpos += ch[i]->title.width + padding;
  }
}

void Renderer::bg_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  SDL_SetRenderDrawColor(rend, r, g, b, a);
}

void Renderer::clear_render(void) { SDL_RenderClear(rend); }

void Renderer::render_present(void) { SDL_RenderPresent(rend); }

SDL_Renderer *Renderer::get_renderer(void) { return renderer_instance.rend; }

const void *Renderer::create_renderer(SDL_Window *w) {
  const int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  rend = SDL_CreateRenderer(w, -1, flags);
  if (!rend) {
    fprintf(stderr, "Failed to create renderer! -> %s\n", SDL_GetError());
    return NULL;
  }
  return rend;
}

Renderer *get_renderer_inst(void) { return &renderer_instance; }
